#include "dicomnetwork.h"

#ifdef ALIZA_USE_DCMTK

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmnet/scu.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include <QDir>
#include <QUuid>

namespace
{

OFString to_of(const QString & s) { return OFString(s.toUtf8().constData()); }
QString from_of(const OFString & s) { return QString::fromUtf8(s.c_str()); }

QString get_tag_string(DcmDataset * ds, const DcmTagKey & tag)
{
	if (!ds) return QString();
	OFString val;
	if (ds->findAndGetOFStringArray(tag, val).good())
		return from_of(val);
	return QString();
}

} // namespace

// C-ECHO
bool DIMSEClient::echo(const DicomNode & node)
{
	DcmSCU scu;
	scu.setPeerHostName(to_of(node.hostname));
	scu.setPeerPort(static_cast<Uint16>(node.port));
	scu.setPeerAETitle(to_of(node.ae_title));
	scu.setAETitle(to_of(node.local_ae_title.isEmpty() ? QString("ALIZAMS") : node.local_ae_title));
	scu.addPresentationContext(UID_VerificationSOPClass, {UID_LittleEndianImplicitTransferSyntax});
	if (scu.initNetwork().bad()) return false;
	if (scu.negotiateAssociation().bad()) return false;
	const bool ok = scu.sendECHORequest(0).good();
	scu.releaseAssociation();
	return ok;
}

// C-FIND
QList<DicomQueryResult> DIMSEClient::find_studies(
	const DicomNode & node,
	const QMap<QString, QString> & filters)
{
	QList<DicomQueryResult> results;
	DcmSCU scu;
	scu.setPeerHostName(to_of(node.hostname));
	scu.setPeerPort(static_cast<Uint16>(node.port));
	scu.setPeerAETitle(to_of(node.ae_title));
	scu.setAETitle(to_of(node.local_ae_title.isEmpty() ? QString("ALIZAMS") : node.local_ae_title));
	OFList<OFString> ts;
	ts.push_back(UID_LittleEndianImplicitTransferSyntax);
	ts.push_back(UID_LittleEndianExplicitTransferSyntax);
	scu.addPresentationContext(UID_FINDStudyRootQueryRetrieveInformationModel, ts);
	if (scu.initNetwork().bad()) return results;
	if (scu.negotiateAssociation().bad()) return results;

	T_ASC_PresentationContextID pc = scu.findPresentationContextID(
		UID_FINDStudyRootQueryRetrieveInformationModel, "");
	if (pc == 0) { scu.releaseAssociation(); return results; }

	DcmDataset query;
	query.putAndInsertOFStringArray(DCM_QueryRetrieveLevel, "STUDY");
	query.putAndInsertOFStringArray(DCM_PatientName,
		to_of(filters.value("PatientName", "*")));
	query.putAndInsertOFStringArray(DCM_PatientID,
		to_of(filters.value("PatientID", "")));
	query.putAndInsertOFStringArray(DCM_StudyDate,
		to_of(filters.value("StudyDate", "")));
	query.putAndInsertOFStringArray(DCM_ModalitiesInStudy,
		to_of(filters.value("ModalitiesInStudy", "")));
	query.putAndInsertOFStringArray(DCM_AccessionNumber,
		to_of(filters.value("AccessionNumber", "")));
	query.putAndInsertOFStringArray(DCM_StudyDescription, "");
	query.putAndInsertOFStringArray(DCM_StudyInstanceUID, "");
	query.putAndInsertOFStringArray(DCM_NumberOfStudyRelatedSeries, "");
	query.putAndInsertOFStringArray(DCM_NumberOfStudyRelatedInstances, "");

	OFList<QRResponse*> responses;
	if (scu.sendFINDRequest(pc, &query, &responses).good())
	{
		for (auto * resp : responses)
		{
			if (resp->m_dataset)
			{
				DicomQueryResult r;
				r.patient_name = get_tag_string(resp->m_dataset, DCM_PatientName);
				r.patient_id = get_tag_string(resp->m_dataset, DCM_PatientID);
				r.study_date = get_tag_string(resp->m_dataset, DCM_StudyDate);
				r.study_description = get_tag_string(resp->m_dataset, DCM_StudyDescription);
				r.study_uid = get_tag_string(resp->m_dataset, DCM_StudyInstanceUID);
				r.accession_number = get_tag_string(resp->m_dataset, DCM_AccessionNumber);
				r.modality = get_tag_string(resp->m_dataset, DCM_ModalitiesInStudy);
				r.num_series = get_tag_string(resp->m_dataset, DCM_NumberOfStudyRelatedSeries).toInt();
				r.num_instances = get_tag_string(resp->m_dataset, DCM_NumberOfStudyRelatedInstances).toInt();
				if (!r.study_uid.isEmpty())
					results.append(r);
			}
		}
	}
	scu.releaseAssociation();
	return results;
}

// C-MOVE (retrieve to local via C-STORE SCP)
bool DIMSEClient::retrieve_study(
	const DicomNode & node,
	const QString & study_uid,
	const QString & output_dir)
{
	// Use C-GET instead of C-MOVE (simpler, no SCP needed)
	DcmSCU scu;
	scu.setPeerHostName(to_of(node.hostname));
	scu.setPeerPort(static_cast<Uint16>(node.port));
	scu.setPeerAETitle(to_of(node.ae_title));
	scu.setAETitle(to_of(node.local_ae_title.isEmpty() ? QString("ALIZAMS") : node.local_ae_title));

	OFList<OFString> ts;
	ts.push_back(UID_LittleEndianImplicitTransferSyntax);
	ts.push_back(UID_LittleEndianExplicitTransferSyntax);
	scu.addPresentationContext(UID_GETStudyRootQueryRetrieveInformationModel, ts);
	// Add storage presentation contexts for CT
	scu.addPresentationContext(UID_CTImageStorage, ts);
	scu.addPresentationContext(UID_MRImageStorage, ts);
	scu.addPresentationContext(UID_SecondaryCaptureImageStorage, ts);
	scu.addPresentationContext(UID_EnhancedCTImageStorage, ts);
	scu.addPresentationContext(UID_EnhancedMRImageStorage, ts);
	scu.addPresentationContext(UID_NuclearMedicineImageStorage, ts);
	scu.addPresentationContext(UID_UltrasoundImageStorage, ts);
	scu.addPresentationContext(UID_DigitalXRayImageStorageForPresentation, ts);
	scu.addPresentationContext(UID_ComputedRadiographyImageStorage, ts);
	scu.addPresentationContext(UID_PositronEmissionTomographyImageStorage, ts);

	if (scu.initNetwork().bad()) return false;
	if (scu.negotiateAssociation().bad()) return false;

	T_ASC_PresentationContextID pc = scu.findPresentationContextID(
		UID_GETStudyRootQueryRetrieveInformationModel, "");
	if (pc == 0) { scu.releaseAssociation(); return false; }

	DcmDataset query;
	query.putAndInsertOFStringArray(DCM_QueryRetrieveLevel, "STUDY");
	query.putAndInsertOFStringArray(DCM_StudyInstanceUID, to_of(study_uid));

	QDir dir(output_dir);
	if (!dir.exists()) dir.mkpath(output_dir);

	OFList<RetrieveResponse*> responses;
	if (scu.sendCGETRequest(pc, &query, &responses).good())
	{
		// Save received datasets
		for (auto * resp : responses)
		{
			if (resp->m_dataset)
			{
				const QString fname = output_dir + "/" +
					QUuid::createUuid().toString(QUuid::WithoutBraces) + ".dcm";
				DcmFileFormat ff(resp->m_dataset);
				ff.saveFile(to_of(fname));
			}
		}
		scu.releaseAssociation();
		return true;
	}
	scu.releaseAssociation();
	return false;
}

// C-STORE
bool DIMSEClient::store(
	const DicomNode & node,
	const QString & file_path)
{
	DcmSCU scu;
	scu.setPeerHostName(to_of(node.hostname));
	scu.setPeerPort(static_cast<Uint16>(node.port));
	scu.setPeerAETitle(to_of(node.ae_title));
	scu.setAETitle(to_of(node.local_ae_title.isEmpty() ? QString("ALIZAMS") : node.local_ae_title));

	DcmFileFormat ff;
	if (ff.loadFile(to_of(file_path)).bad()) return false;

	DcmDataset * ds = ff.getDataset();
	if (!ds) return false;

	OFString sopClass;
	ds->findAndGetOFStringArray(DCM_SOPClassUID, sopClass);
	if (sopClass.empty()) return false;

	OFList<OFString> ts;
	ts.push_back(UID_LittleEndianImplicitTransferSyntax);
	ts.push_back(UID_LittleEndianExplicitTransferSyntax);
	scu.addPresentationContext(sopClass, ts);

	if (scu.initNetwork().bad()) return false;
	if (scu.negotiateAssociation().bad()) return false;

	Uint16 rspStatus = 0;
	const bool ok = scu.sendSTORERequest(0, to_of(file_path), ds, rspStatus).good()
		&& rspStatus == STATUS_Success;
	scu.releaseAssociation();
	return ok;
}

#endif // ALIZA_USE_DCMTK
