
#include "SpidercamView.hpp"
#include "../../Utilities/Constants.hpp"
#include "ExperimentTypes.hpp"
#include "../../Utilities/RappFieldBoundary.hpp"

#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>


cSpidercamView::cSpidercamView()
	:
	mpScanArea(nullptr), mpX_m(nullptr), mpY_m(nullptr), mpZ_m(nullptr),
	mpExperimentStatus(nullptr)
{
	mpScanArea = new cSpidercamScanArea(this);

	QLabel* pX_Label = new QLabel();
	pX_Label->setText("X (m)");

	mpX_m = new QLineEdit();
	mpX_m->setReadOnly(true);

	QLabel* pY_Label = new QLabel();
	pY_Label->setText("Y (m)");

	mpY_m = new QLineEdit();
	mpY_m->setReadOnly(true);

	QLabel* pZ_Label = new QLabel();
	pZ_Label->setText("Z (m)");

	mpZ_m = new QLineEdit();
	mpZ_m->setReadOnly(true);

	mpExperimentStatus = new QStatusBar();
	mpExperimentStatus->setHidden(true);
	mpExperimentStatus->setSizeGripEnabled(false);
	mpExperimentStatus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	mpExperimentStatus->setStyleSheet("background-color: rgb(8, 255, 8);");


	auto* mainlayout = new QVBoxLayout();
	mainlayout->addWidget(mpScanArea);

	auto* statuslayout = new QHBoxLayout();

	statuslayout->addWidget(pX_Label);
	statuslayout->addWidget(mpX_m);
	statuslayout->addWidget(pY_Label);
	statuslayout->addWidget(mpY_m);
	statuslayout->addWidget(pZ_Label);
	statuslayout->addWidget(mpZ_m);

	mainlayout->addLayout(statuslayout);

	mainlayout->addWidget(mpExperimentStatus);

	setLayout(mainlayout);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(500, 500);
	setMaximumSize(16777215, 16777215);
}

cSpidercamView::~cSpidercamView()
{
}

int32_t cSpidercamView::x_mm() const { return mX_mm; }
int32_t cSpidercamView::y_mm() const { return mY_mm; }
int32_t cSpidercamView::z_mm() const { return mZ_mm; }


void cSpidercamView::configure(const nlohmann::json& jsonCfg)
{
	try
	{
		mMinX_mm = jsonCfg["min X position (m)"] * M_TO_MM;
		mMaxX_mm = jsonCfg["max X position (m)"] * M_TO_MM;
		mMinY_mm = jsonCfg["min Y position (m)"] * M_TO_MM;
		mMaxY_mm = jsonCfg["max Y position (m)"] * M_TO_MM;
		mMinHeight_mm = jsonCfg["min height (m)"] * M_TO_MM;
		mMaxHeight_mm = jsonCfg["max height (m)"] * M_TO_MM;

		if (jsonCfg.contains("markers"))
		{
			auto markers = jsonCfg["markers"];

			rfm::rappPoint_t point = rfb::fromGPS(40.0635686 * nConstants::DEG_TO_RAD, -88.2081615 * nConstants::DEG_TO_RAD, 250);

			for (const auto& marker : markers)
			{
				if (marker.contains("x_mm") && marker.contains("y_mm") && marker.contains("z_mm"))
				{
					std::string label = marker["label"];
					int32_t x_mm = marker["x_mm"];
					int32_t y_mm = marker["y_mm"];
					int32_t z_mm = marker["z_mm"];
					mpScanArea->addMarker(label, x_mm, y_mm, z_mm);
				}
				else if (marker.contains("northing_ft") && marker.contains("easting_ft") && marker.contains("height_ft"))
				{
					std::string label = marker["label"];
					double northing_ft = marker["northing_ft"];
					double easting_ft = marker["easting_ft"];
					double height_ft = marker["height_ft"];
					rfm::rappPoint_t point = rfb::fromStatePlane(northing_ft, easting_ft, height_ft);
					mpScanArea->addMarker(label, point.x_mm, point.y_mm, point.z_mm);
				}
				else if (marker.contains("lat_rad") && marker.contains("lng_rad") && marker.contains("height_m"))
				{
					std::string label = marker["label"];
					double lat_rad = marker["lat_rad"];
					double lng_rad = marker["lng_rad"];
					double height_m = marker["height_m"];
					rfm::rappPoint_t point = rfb::fromGPS(lat_rad, lng_rad, height_m);
					mpScanArea->addMarker(label, point.x_mm, point.y_mm, point.z_mm);
				}
			}

		}

		if (jsonCfg.contains("layout"))
		{
			mLayoutFilename = jsonCfg["layout"];
			mpScanArea->loadLayout(mLayoutFilename);
		}
	}
	catch (const std::exception& e)
	{
	}

	mpScanArea->updateBounds(mMinX_mm, mMaxX_mm, mMinY_mm, mMaxY_mm);
	mpScanArea->updateDollyPosition((mMaxX_mm - mMinX_mm)/2.0 + mMinX_mm, (mMaxY_mm - mMinY_mm) / 2.0 + mMinY_mm);
}

void cSpidercamView::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;

	mpScanArea->updateBounds(mMinX_mm, mMaxX_mm, mMinY_mm, mMaxY_mm);
}

void cSpidercamView::updatePosition(spidercam::sPosition_1_t pos)
{
	mpScanArea->updateDollyPosition(pos.X_mm, pos.Y_mm);

	mX_mm = pos.X_mm;
	mY_mm = pos.Y_mm;
	mZ_mm = pos.Z_mm;

	mpX_m->setText(QString::number(pos.X_mm * nConstants::MM_TO_M, 'f', 3));
	mpY_m->setText(QString::number(pos.Y_mm * nConstants::MM_TO_M, 'f', 3));
	mpZ_m->setText(QString::number(pos.height_mm * nConstants::MM_TO_M, 'f', 3));
}

void cSpidercamView::updateRecordingState(bool recording)
{
	mpScanArea->setRecording(recording);
}

void cSpidercamView::refresh()
{
	if (mLayoutFilename.empty()) return;

	mpScanArea->loadLayout(mLayoutFilename);
}

void cSpidercamView::experimentStateChanging(experiment::eState state)
{
	using namespace experiment;

	switch (state)
	{
	case eState::LOADED:
		mpExperimentStatus->setHidden(false);
		break;
	case eState::RUNNING:
		mpExperimentStatus->setStyleSheet("background-color: rgb(8, 255, 8);");
		break;
	case eState::PAUSED:
		mpExperimentStatus->setStyleSheet("background-color: rgb(255, 191, 0);");
		break;
	case eState::COMPLETED:
	case eState::TERMINATED:
		mpExperimentStatus->clearMessage();
		mpExperimentStatus->setHidden(true);
		break;
	case eState::EXP_ERROR:
		mpExperimentStatus->setStyleSheet("background-color: rgb(235, 33, 46);");
		break;
	}
}

void cSpidercamView::experimentStatusUpdating(QString msg)
{
	mpExperimentStatus->showMessage(msg);
}
