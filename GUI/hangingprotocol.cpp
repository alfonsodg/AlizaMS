#include "hangingprotocol.h"
#include <QRegularExpression>
#include <cmath>

HangingProtocolEngine & HangingProtocolEngine::instance()
{
	static HangingProtocolEngine e;
	return e;
}

HangingProtocolEngine::HangingProtocolEngine()
{
	load_builtin();
}

void HangingProtocolEngine::load_builtin()
{
	// 1. CT Chest: lung + mediastinal windows
	{
		HangingProtocol p;
		p.name = QString("CT Chest");
		p.modality = QString("CT");
		p.body_part = QString("CHEST");
		p.description_regex = QString("chest|thorax|lung");
		p.priority = 10;
		p.grid_rows = 1;
		p.grid_cols = 2;
		SeriesAssignment lung;
		lung.viewport_index = 0;
		lung.description_match = QString(".*");
		lung.window_center = -600.0;
		lung.window_width = 1500.0;
		lung.use_default_wl = false;
		SeriesAssignment mediastinal;
		mediastinal.viewport_index = 1;
		mediastinal.description_match = QString(".*");
		mediastinal.window_center = 40.0;
		mediastinal.window_width = 400.0;
		mediastinal.use_default_wl = false;
		p.assignments.push_back(lung);
		p.assignments.push_back(mediastinal);
		m_protocols.push_back(p);
	}
	// 2. CT Abdomen: soft tissue windows
	{
		HangingProtocol p;
		p.name = QString("CT Abdomen");
		p.modality = QString("CT");
		p.body_part = QString("ABDOMEN");
		p.description_regex = QString("abdomen|abdominal|pelvis");
		p.priority = 10;
		p.grid_rows = 1;
		p.grid_cols = 3;
		SeriesAssignment soft;
		soft.viewport_index = 0;
		soft.description_match = QString(".*");
		soft.window_center = 40.0;
		soft.window_width = 400.0;
		soft.use_default_wl = false;
		SeriesAssignment liver;
		liver.viewport_index = 1;
		liver.description_match = QString(".*");
		liver.window_center = 70.0;
		liver.window_width = 150.0;
		liver.use_default_wl = false;
		SeriesAssignment bone;
		bone.viewport_index = 2;
		bone.description_match = QString(".*");
		bone.window_center = 400.0;
		bone.window_width = 1800.0;
		bone.use_default_wl = false;
		p.assignments.push_back(soft);
		p.assignments.push_back(liver);
		p.assignments.push_back(bone);
		m_protocols.push_back(p);
	}
	// 3. MR Brain: T1/T2/FLAIR/DWI
	{
		HangingProtocol p;
		p.name = QString("MR Brain");
		p.modality = QString("MR");
		p.body_part = QString("HEAD");
		p.description_regex = QString("brain|head|neuro");
		p.priority = 10;
		p.grid_rows = 2;
		p.grid_cols = 2;
		SeriesAssignment t1;
		t1.viewport_index = 0;
		t1.description_match = QString("T1|t1w|T1W");
		SeriesAssignment t2;
		t2.viewport_index = 1;
		t2.description_match = QString("T2|t2w|T2W");
		SeriesAssignment flair;
		flair.viewport_index = 2;
		flair.description_match = QString("FLAIR|flair|Flair");
		SeriesAssignment dwi;
		dwi.viewport_index = 3;
		dwi.description_match = QString("DWI|dwi|diffusion|DIFFUSION");
		p.assignments.push_back(t1);
		p.assignments.push_back(t2);
		p.assignments.push_back(flair);
		p.assignments.push_back(dwi);
		m_protocols.push_back(p);
	}
	// 4. Mammography: CC+MLO bilateral
	{
		HangingProtocol p;
		p.name = QString("Mammography");
		p.modality = QString("MG");
		p.body_part = QString("BREAST");
		p.description_regex = QString("mammo|breast");
		p.priority = 10;
		p.grid_rows = 2;
		p.grid_cols = 2;
		SeriesAssignment rcc;
		rcc.viewport_index = 0;
		rcc.description_match = QString("R.*CC|RCC");
		SeriesAssignment lcc;
		lcc.viewport_index = 1;
		lcc.description_match = QString("L.*CC|LCC");
		SeriesAssignment rmlo;
		rmlo.viewport_index = 2;
		rmlo.description_match = QString("R.*MLO|RMLO");
		SeriesAssignment lmlo;
		lmlo.viewport_index = 3;
		lmlo.description_match = QString("L.*MLO|LMLO");
		p.assignments.push_back(rcc);
		p.assignments.push_back(lcc);
		p.assignments.push_back(rmlo);
		p.assignments.push_back(lmlo);
		m_protocols.push_back(p);
	}
	// 5. PET/CT: fused+CT+PET+MIP
	{
		HangingProtocol p;
		p.name = QString("PET/CT");
		p.modality = QString("PT");
		p.body_part = QString("");
		p.description_regex = QString("PET|pet|pet.ct|PET.CT");
		p.priority = 10;
		p.grid_rows = 2;
		p.grid_cols = 2;
		SeriesAssignment fused;
		fused.viewport_index = 0;
		fused.description_match = QString("fuse|fusion|FUSED");
		SeriesAssignment ct;
		ct.viewport_index = 1;
		ct.description_match = QString("CT|ct.*corr");
		ct.window_center = 40.0;
		ct.window_width = 400.0;
		ct.use_default_wl = false;
		SeriesAssignment pet;
		pet.viewport_index = 2;
		pet.description_match = QString("PET|pet|SUV");
		SeriesAssignment mip;
		mip.viewport_index = 3;
		mip.description_match = QString("MIP|mip");
		p.assignments.push_back(fused);
		p.assignments.push_back(ct);
		p.assignments.push_back(pet);
		p.assignments.push_back(mip);
		m_protocols.push_back(p);
	}
	// 6. General fallback: auto-grid based on series count
	{
		HangingProtocol p;
		p.name = QString("General");
		p.modality = QString("*");
		p.body_part = QString("");
		p.description_regex = QString("");
		p.priority = 0;
		p.grid_rows = 1;
		p.grid_cols = 1;
		m_protocols.push_back(p);
	}
}

const HangingProtocol * HangingProtocolEngine::match(
	const QString & modality,
	const QString & body_part,
	const QString & study_description,
	int num_series) const
{
	const HangingProtocol * best = nullptr;
	int best_score = -1;
	const HangingProtocol * general = nullptr;
	for (int i = 0; i < m_protocols.size(); ++i)
	{
		const HangingProtocol & p = m_protocols.at(i);
		// Track the general fallback
		if (p.modality == QString("*"))
		{
			general = &p;
			continue;
		}
		int score = 0;
		// Modality must match
		if (p.modality.compare(modality, Qt::CaseInsensitive) != 0)
			continue;
		score += 10;
		// Body part match (bonus)
		if (!p.body_part.isEmpty() &&
			p.body_part.compare(body_part, Qt::CaseInsensitive) == 0)
		{
			score += 5;
		}
		// Study description regex match (bonus)
		if (!p.description_regex.isEmpty() && !study_description.isEmpty())
		{
			const QRegularExpression re(
				p.description_regex,
				QRegularExpression::CaseInsensitiveOption);
			if (re.isValid() && re.match(study_description).hasMatch())
			{
				score += 3;
			}
		}
		// Prefer higher priority protocols when scores are equal
		if (score > best_score ||
			(score == best_score && best && p.priority > best->priority))
		{
			best_score = score;
			best = &p;
		}
	}
	if (best)
		return best;
	// Adjust general fallback grid based on series count
	if (general && num_series > 1)
	{
		// Return general as-is; caller should compute grid from num_series
		return general;
	}
	return general;
}

const QList<HangingProtocol> & HangingProtocolEngine::protocols() const
{
	return m_protocols;
}

void HangingProtocolEngine::add(const HangingProtocol & protocol)
{
	m_protocols.push_back(protocol);
}
