#include "dicomnetwork.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QDir>
#include <QUuid>

namespace
{

// Synchronous HTTP request helper
QNetworkReply * sync_request(
	QNetworkAccessManager & mgr,
	const QNetworkRequest & req,
	const QByteArray & verb = QByteArray(),
	const QByteArray & body = QByteArray())
{
	QNetworkReply * reply;
	if (!verb.isEmpty() && verb == "POST")
		reply = mgr.post(req, body);
	else
		reply = mgr.get(req);
	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	return reply;
}

void apply_auth(QNetworkRequest & req, const DicomNode & node)
{
	if (!node.auth_token.isEmpty())
		req.setRawHeader("Authorization", ("Bearer " + node.auth_token).toUtf8());
}

// Extract first value string from a DICOM JSON tag object:
// { "00100010": { "vr": "PN", "Value": [{"Alphabetic":"..."}] } }
// or { "00080020": { "vr": "DA", "Value": ["20240101"] } }
QString tag_value(const QJsonObject & obj, const QString & tag)
{
	if (!obj.contains(tag)) return QString();
	const QJsonObject t = obj[tag].toObject();
	const QJsonArray vals = t.value("Value").toArray();
	if (vals.isEmpty()) return QString();
	const QJsonValue first = vals.at(0);
	if (first.isString()) return first.toString();
	if (first.isObject())
	{
		// PN type: { "Alphabetic": "..." }
		const QJsonObject pn = first.toObject();
		if (pn.contains("Alphabetic")) return pn["Alphabetic"].toString();
	}
	if (first.isDouble()) return QString::number(first.toInt());
	return QString();
}

int tag_int(const QJsonObject & obj, const QString & tag)
{
	const QString v = tag_value(obj, tag);
	if (v.isEmpty()) return 0;
	bool ok = false;
	const int r = v.toInt(&ok);
	return ok ? r : 0;
}

} // namespace

bool DICOMwebClient::echo(const DicomNode & node)
{
	if (node.qido_url.isEmpty()) return false;
	QNetworkAccessManager mgr;
	QUrl url(node.qido_url + "/studies");
	QUrlQuery q;
	q.addQueryItem("limit", "1");
	url.setQuery(q);
	QNetworkRequest req(url);
	req.setRawHeader("Accept", "application/dicom+json");
	apply_auth(req, node);
	QNetworkReply * reply = sync_request(mgr, req);
	const bool ok = (reply->error() == QNetworkReply::NoError &&
		reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200);
	reply->deleteLater();
	return ok;
}

QList<DicomQueryResult> DICOMwebClient::find_studies(
	const DicomNode & node,
	const QMap<QString, QString> & filters)
{
	QList<DicomQueryResult> results;
	if (node.qido_url.isEmpty()) return results;
	// Map user-friendly filter keys to DICOM tags
	static const QMap<QString, QString> filter_map = {
		{"PatientName",        "PatientName"},
		{"PatientID",          "PatientID"},
		{"StudyDate",          "StudyDate"},
		{"ModalitiesInStudy",  "ModalitiesInStudy"},
		{"AccessionNumber",    "AccessionNumber"}
	};
	QNetworkAccessManager mgr;
	QUrl url(node.qido_url + "/studies");
	QUrlQuery q;
	for (auto it = filters.constBegin(); it != filters.constEnd(); ++it)
	{
		const QString key = filter_map.value(it.key(), it.key());
		q.addQueryItem(key, it.value());
	}
	url.setQuery(q);
	QNetworkRequest req(url);
	req.setRawHeader("Accept", "application/dicom+json");
	apply_auth(req, node);
	QNetworkReply * reply = sync_request(mgr, req);
	if (reply->error() != QNetworkReply::NoError)
	{
		reply->deleteLater();
		return results;
	}
	const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
	reply->deleteLater();
	if (!doc.isArray()) return results;
	const QJsonArray arr = doc.array();
	for (int i = 0; i < arr.size(); ++i)
	{
		const QJsonObject obj = arr.at(i).toObject();
		DicomQueryResult r;
		r.patient_name      = tag_value(obj, "00100010"); // PatientName
		r.patient_id        = tag_value(obj, "00100020"); // PatientID
		r.study_date        = tag_value(obj, "00080020"); // StudyDate
		r.study_description = tag_value(obj, "00081030"); // StudyDescription
		r.study_uid         = tag_value(obj, "0020000D"); // StudyInstanceUID
		r.accession_number  = tag_value(obj, "00080050"); // AccessionNumber
		r.modality          = tag_value(obj, "00080061"); // ModalitiesInStudy
		r.num_series        = tag_int(obj,   "00201206"); // NumberOfStudyRelatedSeries
		r.num_instances     = tag_int(obj,   "00201208"); // NumberOfStudyRelatedInstances
		results.append(r);
	}
	return results;
}

bool DICOMwebClient::retrieve_study(
	const DicomNode & node,
	const QString & study_uid,
	const QString & output_dir)
{
	if (node.wado_url.isEmpty() || study_uid.isEmpty()) return false;
	QDir dir(output_dir);
	if (!dir.exists()) dir.mkpath(".");
	QNetworkAccessManager mgr;
	QUrl url(node.wado_url + "/studies/" + study_uid);
	QNetworkRequest req(url);
	req.setRawHeader("Accept", "multipart/related; type=application/dicom");
	apply_auth(req, node);
	QNetworkReply * reply = sync_request(mgr, req);
	if (reply->error() != QNetworkReply::NoError)
	{
		reply->deleteLater();
		return false;
	}
	// Parse multipart/related response
	const QByteArray content_type = reply->rawHeader("Content-Type");
	const QByteArray data = reply->readAll();
	reply->deleteLater();
	// Extract boundary from Content-Type
	QByteArray boundary;
	const int bi = content_type.indexOf("boundary=");
	if (bi < 0) return false;
	boundary = content_type.mid(bi + 9);
	if (boundary.startsWith('"') && boundary.endsWith('"'))
		boundary = boundary.mid(1, boundary.size() - 2);
	const QByteArray delim = "--" + boundary;
	const QList<QByteArray> parts = data.split(delim.constData());
	int saved = 0;
	for (int i = 0; i < parts.size(); ++i)
	{
		const QByteArray & part = parts.at(i);
		if (part.trimmed().isEmpty() || part.trimmed() == "--") continue;
		// Find the empty line separating headers from body
		const int sep = part.indexOf("\r\n\r\n");
		if (sep < 0) continue;
		const QByteArray body = part.mid(sep + 4);
		if (body.isEmpty()) continue;
		const QString fname = output_dir + "/" + QUuid::createUuid().toString(QUuid::Id128) + ".dcm";
		QFile f(fname);
		if (f.open(QIODevice::WriteOnly))
		{
			f.write(body);
			f.close();
			++saved;
		}
	}
	return (saved > 0);
}

bool DICOMwebClient::store(
	const DicomNode & node,
	const QString & file_path)
{
	if (node.stow_url.isEmpty()) return false;
	QFile f(file_path);
	if (!f.open(QIODevice::ReadOnly)) return false;
	const QByteArray file_data = f.readAll();
	f.close();
	const QByteArray boundary = QUuid::createUuid().toByteArray(QUuid::Id128);
	QByteArray body;
	body.append("--" + boundary + "\r\n");
	body.append("Content-Type: application/dicom\r\n\r\n");
	body.append(file_data);
	body.append("\r\n--" + boundary + "--\r\n");
	QNetworkAccessManager mgr;
	QUrl url(node.stow_url + "/studies");
	QNetworkRequest req(url);
	req.setRawHeader("Content-Type",
		"multipart/related; type=application/dicom; boundary=" + boundary);
	apply_auth(req, node);
	QNetworkReply * reply = sync_request(mgr, req, "POST", body);
	const bool ok = (reply->error() == QNetworkReply::NoError);
	reply->deleteLater();
	return ok;
}
