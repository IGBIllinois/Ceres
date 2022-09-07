
#include "SpidercamView.hpp"
#include "../../Utilities/Constants.hpp"
#include "../ExperimentTypes.hpp"

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
