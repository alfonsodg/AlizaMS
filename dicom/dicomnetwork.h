#ifndef A_DICOMNETWORK_H
#define A_DICOMNETWORK_H

#include <QString>
#include <QList>
#include <QMap>
#include <QByteArray>

// DICOM network node configuration
struct DicomNode
{
	QString name;
	QString ae_title;
	QString hostname;
	int port{104};
	bool use_tls{false};
	// DICOMweb
	QString wado_url;    // e.g. https://server/wado-rs
	QString qido_url;    // e.g. https://server/qido-rs
	QString stow_url;    // e.g. https://server/stow-rs
	QString auth_token;  // Bearer token for DICOMweb
};

// Query result entry
struct DicomQueryResult
{
	QString patient_name;
	QString patient_id;
	QString study_date;
	QString study_description;
	QString study_uid;
	QString accession_number;
	QString modality;
	int num_series{0};
	int num_instances{0};
};

// Abstract DICOM network interface
class DicomNetwork
{
public:
	virtual ~DicomNetwork() = default;

	// Echo (verify connectivity)
	virtual bool echo(const DicomNode & node) = 0;

	// Query studies
	virtual QList<DicomQueryResult> find_studies(
		const DicomNode & node,
		const QMap<QString, QString> & filters) = 0;

	// Retrieve a study to local directory
	virtual bool retrieve_study(
		const DicomNode & node,
		const QString & study_uid,
		const QString & output_dir) = 0;

	// Store a DICOM file to remote node
	virtual bool store(
		const DicomNode & node,
		const QString & file_path) = 0;
};

// DICOMweb implementation using Qt Network
class DICOMwebClient : public DicomNetwork
{
public:
	bool echo(const DicomNode & node) override;
	QList<DicomQueryResult> find_studies(
		const DicomNode & node,
		const QMap<QString, QString> & filters) override;
	bool retrieve_study(
		const DicomNode & node,
		const QString & study_uid,
		const QString & output_dir) override;
	bool store(
		const DicomNode & node,
		const QString & file_path) override;
};

#endif
