
#include "HySpexStatusView.hpp"
#include "HySpexCameraModel.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>

#include <QCustomPlot/qcustomplot.h>

#include <string>

/** ERROR is define in Windows */
#ifdef ERROR
	#undef ERROR
#endif

cHySpexStatusView::cHySpexStatusView(cHySpexCameraModel* pModel, QWidget* parent)
:
    cSensorStatusView(pModel, parent), mpModel(pModel)
{
}

cHySpexStatusView::~cHySpexStatusView()
{
}

void cHySpexStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpInitializationStatus = new QButtonIndicator(this);
	mpInitializationStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpInitializationStatus->setMinimumWidth(125);

	mpAcquisitionStatus = new QButtonIndicator(this);
	mpAcquisitionStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpAcquisitionStatus->setMinimumWidth(125);

	mpBackgroundStatus = new QButtonIndicator(this);
	mpBackgroundStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpBackgroundStatus->setMinimumWidth(125);

	mpCommunicationStatus = new QButtonIndicator(this);
	mpCommunicationStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpCommunicationStatus->setMinimumWidth(125);

	mpCoolingStatus = new QButtonIndicator(this);
	mpCoolingStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpCoolingStatus->setMinimumWidth(125);

	mpShutterStatus = new QButtonIndicator(this);
	mpShutterStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpShutterStatus->setMinimumWidth(125);

	/** Acquisition Info */
	mpAvgFramesLabel = new QLabel("Avg Frames", this);
	mpAvgFrames = new QLineEdit(this);
	mpAvgFrames->setReadOnly(true);

	mpFramePeriodLabel = new QLabel("Frame Period (us) :", this);
	mpFramePeriod_us = new QLineEdit(this);
	mpFramePeriod_us->setReadOnly(true);

	mpMinFramePeriodLabel = new QLabel("Min Frame Period (us) :", this);
	mpMinFramePeriod_us = new QLineEdit(this);
	mpMinFramePeriod_us->setReadOnly(true);

	mpIntegrationTimeLabel = new QLabel("Integration Time (us) :", this);
	mpIntegrationTime_us = new QLineEdit(this);
	mpIntegrationTime_us->setReadOnly(true);

	mpMaxIntegrationTimeLabel = new QLabel("Max Integration Time (us) :", this);
	mpMaxIntegrationTime_us = new QLineEdit(this);
	mpMaxIntegrationTime_us->setReadOnly(true);

	mpAmbientTempLabel = new QLabel("Ambient Temp (C) :", this);
	mpAmbientTemp_C = new QLineEdit(this);
	mpAmbientTemp_C->setReadOnly(true);

	/** Lens Info */
	mLensNameLabel = new QLabel("Name");
	mpLensName = new QLineEdit();
	mpLensName->setReadOnly(true);

	mLensWorkingDistanceLabel = new QLabel("WD (cm)");
	mpLensWorkingDistance_cm = new QLineEdit();
	mpLensWorkingDistance_cm->setReadOnly(true);

	mLensFieldOfViewLabel = new QLabel("FOV (deg)");
	mpLensFieldOfView_deg = new QLineEdit();
	mpLensFieldOfView_deg->setReadOnly(true);

	/** Display */
	mpSaturationButton = new QPushButton("%SAT");
	mpSaturationButton->setCheckable(true);
	QObject::connect(mpSaturationButton, &QPushButton::clicked, this, &cHySpexStatusView::saturationButtonToggled);

	mpPlot = new QCustomPlot();
	mpPlot->setMinimumHeight(300);
	mpPlot->addGraph();

//	customPlot->graph(0)->setData(x, y);
	// give the axes some labels:
//	customPlot->xAxis->setLabel("x");
//	customPlot->yAxis->setLabel("y");
	// set axes ranges, so we see all data:
	mpPlot->xAxis->setRange(0, 1);
	mpPlot->yAxis->setRange(0, 1);

}

void cHySpexStatusView::doStatusLayout(QBoxLayout* pMainLayout)
{
	QGroupBox* cameraStatusBox = new QGroupBox("Camera Status");
	cameraStatusBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto* statusLayout = new QHBoxLayout();

	statusLayout->addWidget(mpInitializationStatus);
	statusLayout->addWidget(mpCommunicationStatus);
	statusLayout->addWidget(mpAcquisitionStatus);
	statusLayout->addWidget(mpCoolingStatus);
	statusLayout->addWidget(mpBackgroundStatus);
	statusLayout->addWidget(mpShutterStatus);

	cameraStatusBox->setLayout(statusLayout);

	pMainLayout->addWidget(cameraStatusBox);
}

void cHySpexStatusView::doAcqStatusLayout(QBoxLayout* pMainLayout)
{
	QGroupBox* acqBox = new QGroupBox("Acquisition Status");
	acqBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto* acqLayout = new QHBoxLayout();

	acqLayout->addWidget(mpAvgFramesLabel);
	acqLayout->addWidget(mpAvgFrames);

	acqLayout->addWidget(mpFramePeriodLabel);
	acqLayout->addWidget(mpFramePeriod_us);

	acqLayout->addWidget(mpMinFramePeriodLabel);
	acqLayout->addWidget(mpMinFramePeriod_us);

	acqLayout->addWidget(mpIntegrationTimeLabel);
	acqLayout->addWidget(mpIntegrationTime_us);

	acqLayout->addWidget(mpMaxIntegrationTimeLabel);
	acqLayout->addWidget(mpMaxIntegrationTime_us);

	acqLayout->addWidget(mpAmbientTempLabel);
	acqLayout->addWidget(mpAmbientTemp_C);

	acqBox->setLayout(acqLayout);

	pMainLayout->addWidget(acqBox);
}

void cHySpexStatusView::doLensInfoLayout(QBoxLayout* pMainLayout)
{
	QGroupBox* lensInfoBox = new QGroupBox("Lens Info");
	lensInfoBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	auto* infoLayout = new QHBoxLayout();

	infoLayout->addWidget(mLensNameLabel);
	infoLayout->addWidget(mpLensName);

	infoLayout->addWidget(mLensWorkingDistanceLabel);
	infoLayout->addWidget(mpLensWorkingDistance_cm);

	infoLayout->addWidget(mLensFieldOfViewLabel);
	infoLayout->addWidget(mpLensFieldOfView_deg);

	lensInfoBox->setLayout(infoLayout);

	pMainLayout->addWidget(lensInfoBox);
}

void cHySpexStatusView::doPlotLayout(QBoxLayout* pMainLayout)
{
	auto* plotLayout = new QVBoxLayout();

	auto* buttonLayout = new QHBoxLayout();

	buttonLayout->addWidget(mpSaturationButton);

	plotLayout->addLayout(buttonLayout);

	plotLayout->addWidget(mpPlot);

	pMainLayout->addLayout(plotLayout);
}

void cHySpexStatusView::onInitStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getInitStatus();

	switch (status)
	{
	case HYSPEX_INIT_PENDING_DETECTION:   //!< Camera detection pending.
		mpInitializationStatus->setState(QButtonIndicator::WARNING, "INIT DETECTION");
		break;
	case HYSPEX_INIT_PENDING_ELECTRONICS: //!< Pending initialization of I/O electronics.
		mpInitializationStatus->setState(QButtonIndicator::WARNING, "INIT ELEC");
		break;
	case HYSPEX_INIT_PENDING_SENSOR:      //!< Pending initialization of Sensor.
		mpInitializationStatus->setState(QButtonIndicator::WARNING, "INIT SENSOR");
		break;
	case HYSPEX_INIT_PENDING_TRANSPORT:   //!< Pending initialization of frame grabber (if applicable).
		mpInitializationStatus->setState(QButtonIndicator::WARNING, "INIT TRANSPORT");
		break;
	case HYSPEX_INIT_OK:                  //!< Camera initialization OK.
		mpInitializationStatus->setState(QButtonIndicator::OK, "INIT OK");
		break;
	case HYSPEX_INIT_FAILED_DETECTION:   //!< Camera detection failed. Internal use only, as the camera will not be returned by CameraManager in this state.
		mpInitializationStatus->setState(QButtonIndicator::ERROR, "INIT DETECTION");
		break;
	case HYSPEX_INIT_FAILED_ELECTRONICS: //!< Failed to initialize I/O Electronics.
		mpInitializationStatus->setState(QButtonIndicator::ERROR, "INIT ELEC");
		break;
	case HYSPEX_INIT_FAILED_SENSOR:      //!< Failed to initialize Sensor. Ensure settings are valid.
		mpInitializationStatus->setState(QButtonIndicator::ERROR, "INIT SENSOR");
		break;
	case HYSPEX_INIT_FAILED_TRANSPORT:   //!< Failed to initialize frame grabber (if applicable). Check frame grabber configuration (name, 3-tap vs. 10-tap etc).
		mpInitializationStatus->setState(QButtonIndicator::ERROR, "INIT TRANSPORT");
		break;
	case HYSPEX_INIT_NOT_STARTED:       //!< Canmera::init() has not been called yet. Call Camera::init()
		mpInitializationStatus->setState(QButtonIndicator::ERROR, "INIT NOT STARTED");
		break;
	}
}

void cHySpexStatusView::onAcqStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getAcquisitionStatus();

	switch (status)
	{
	case HYSPEX_ACQ_STOPPED:
		mpAcquisitionStatus->setState(QButtonIndicator::ALERT, "ACQ STOPPED");
		break;
	case HYSPEX_ACQ_PENDING:
		mpAcquisitionStatus->setState(QButtonIndicator::WARNING, "ACQ PENDING");
		break;
	case HYSPEX_ACQ_RUNNING:
		mpAcquisitionStatus->setState(QButtonIndicator::OK, "ACQ STARTED");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_OVERFLOW:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ OVERFLOW");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_TRASH:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ TRASH");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_ERROR:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ ERROR");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_FRAME_PERIOD_MISMATCH:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ MISMATCH");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_TIMEOUT:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ TIMEOUT");
		break;
	case HYSPEX_ACQ_TERMINATED_DUE_TO_CRITICAL_READER_TOO_SLOW:
		mpAcquisitionStatus->setState(QButtonIndicator::ERROR, "ACQ SLOW");
		break;
	}
}

void cHySpexStatusView::onBgStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getBackgroundStatus();

	switch (status)
	{
	case HYSPEX_BG_INVALID:
		mpBackgroundStatus->setState(QButtonIndicator::eState::UNKNOWN, "BG NONE");
		break;
	case HYSPEX_BG_PENDING:
		mpBackgroundStatus->setState(QButtonIndicator::eState::WARNING, "BG PENDING");
		break;
	case HYSPEX_BG_VALID:
		mpBackgroundStatus->setState(QButtonIndicator::eState::OK, "BG OK");
		break;
	case HYSPEX_BG_PENDING_READY:
		mpBackgroundStatus->setState(QButtonIndicator::eState::ACTIVE, "BG OK/CALC");
		break;
	case HYSPEX_BG_EXPIRED:
		mpBackgroundStatus->setState(QButtonIndicator::eState::ALERT, "BG EXPIRED");
		break;
	case HYSPEX_BG_ABORTED:
		mpBackgroundStatus->setState(QButtonIndicator::eState::ERROR, "BG ABORTED");
		break;
	}
}

void cHySpexStatusView::onCommStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getCommunicationStatus();

	switch (status)
	{
	case HYSPEX_COMM_INIT:
		mpCommunicationStatus->setState(QButtonIndicator::UNKNOWN, "COMM INIT");
		break;
	case HYSPEX_COMM_OK:
		mpCommunicationStatus->setState(QButtonIndicator::OK, "COMM OK");
		break;
	case HYSPEX_COMM_FAILED_ELECTRONICS:
		mpCommunicationStatus->setState(QButtonIndicator::ERROR, "COMM ELEC");
		break;
	case HYSPEX_COMM_FAILED_SENSOR:
		mpCommunicationStatus->setState(QButtonIndicator::ERROR, "COMM SENSOR");
		break;
	case HYSPEX_COMM_FAILED_ELECTRONICS_AND_SENSOR:
		mpCommunicationStatus->setState(QButtonIndicator::ERROR, "COMM BOTH");
		break;
	}
}

void cHySpexStatusView::onCoolingStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getCoolingStatus();

	switch (status)
	{
	case HYSPEX_COOLING_STABLE_OK:
		mpCoolingStatus->setState(QButtonIndicator::OK);
		break;
	case HYSPEX_COOLING_NOT_STABILIZED:
		mpCoolingStatus->setState(QButtonIndicator::WARNING);
		break;
	case HYSPEX_COOLING_STABLE_DEGRADED:
		mpCoolingStatus->setState(QButtonIndicator::ALERT);
		break;
	case HYSPEX_COOLING_OUT_OF_RANGE:
		mpCoolingStatus->setState(QButtonIndicator::ERROR);
		break;
	case HYSPEX_COOLING_FAILED_COMMUNICATION:
		mpCoolingStatus->setState(QButtonIndicator::ERROR);
		break;
	case HYSPEX_COOLING_UNKNOWN:
		mpCoolingStatus->setState(QButtonIndicator::UNKNOWN);
		break;
	}
}

void cHySpexStatusView::onShutterStatusChange()
{
	using namespace hyspex;

	auto status = mpModel->getShutterStatus();

	switch (status)
	{
	case HYSPEX_SHUTTER_OPEN:
		mpShutterStatus->setState(QButtonIndicator::OK, "SH OPEN");
		break;
	case HYSPEX_SHUTTER_CLOSED:
		mpShutterStatus->setState(QButtonIndicator::ACTIVE, "SH CLOSE");
		break;
	case HYSPEX_SHUTTER_PENDING_OPEN:
		mpShutterStatus->setState(QButtonIndicator::WARNING, "SH OPEN");
		break;
	case HYSPEX_SHUTTER_PENDING_CLOSE:
		mpShutterStatus->setState(QButtonIndicator::WARNING, "SH CLOSE");
		break;
	case HYSPEX_SHUTTER_FAIL_OPEN:
		mpShutterStatus->setState(QButtonIndicator::ERROR, "SH OPEN");
		break;
	case HYSPEX_SHUTTER_FAIL_CLOSE:
		mpShutterStatus->setState(QButtonIndicator::ERROR, "SH CLOSE");
		break;
	case HYSPEX_SHUTTER_UNKNOWN:
		mpShutterStatus->setState(QButtonIndicator::UNKNOWN, "SH UNKNOWN");
		break;
	}
}


void cHySpexStatusView::onAvgFramesChange(std::uint16_t avgFrames)
{
	mpAvgFrames->setText(QString::number(avgFrames));
}

void cHySpexStatusView::onFramePeriodChange(std::uint32_t period_us)
{
	mpFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexStatusView::onMinFramePeriodChange(std::uint32_t period_us)
{
	mpMinFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexStatusView::onIntegrationTimeChange(std::uint32_t time_us)
{
	mpIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexStatusView::onMaxIntegrationTimeChange(std::uint32_t time_us)
{
	mpMaxIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexStatusView::onAmbientTempChange(double temp_C)
{
	mpAmbientTemp_C->setText(QString::number(temp_C, 'f', 1));
}

void cHySpexStatusView::onSensorTempChange(double temp_C)
{
	QString label = QString::number(temp_C, 'f', 1);
	label += " C";
	mpCoolingStatus->setText(label);
}

void cHySpexStatusView::onLensInfoChange()
{
	auto name = mpModel->getLensName();
	auto wd_cm = mpModel->getWorkingDistance_cm();
	auto fov_deg = mpModel->getFieldOfView_deg();

	mpLensName->setText(QString::fromStdString(name));
	mpLensWorkingDistance_cm->setText(QString::number(wd_cm, 'f', 1));
	mpLensFieldOfView_deg->setText(QString::number(fov_deg, 'f', 1));
}

void cHySpexStatusView::saturationButtonToggled(bool state)
{
	mShowSaturation = state;
	if (mShowSaturation)
	{
		mpPlot->yAxis->setRange(0, 100);
		mpPlot->yAxis->setLabel("% Saturation");

		auto chn = mpModel->getSpatialSize();
		auto bands = mpModel->getSpectralSize();

		mpPlot->xAxis->setRange(0, chn);
		mpPlot->xAxis->setLabel("Channel");
		mpPlot->replot();
	}
}

