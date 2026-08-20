
#include "HySpexCamera_StatusView.hpp"
#include "HySpexCamera_Model.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QDebug>

#include <QCustomPlot/qcustomplot.h>

#include <string>

/** ERROR is define in Windows */
#ifdef ERROR
	#undef ERROR
#endif

namespace
{
	constexpr int MAX_UPDATE_RATE_MSEC = 100;
	constexpr int FOCUS_UPDATE_RATE_MSEC = 100;
}

cHySpexCamera_StatusView::cHySpexCamera_StatusView(cHySpexCameraModel* pModel, QWidget* parent)
:
    cSensorStatusView(pModel, parent), mpModel(pModel)
{
}

cHySpexCamera_StatusView::~cHySpexCamera_StatusView()
{
}

void cHySpexCamera_StatusView::createWidgets()
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
	mpPercentSaturationButton = new QPushButton("%SAT");
	mpPercentSaturationButton->setCheckable(true);
	QObject::connect(mpPercentSaturationButton, &QPushButton::clicked, this, &cHySpexCamera_StatusView::saturationButtonToggled);

	mpPercentBandButton = new QPushButton("%BAND");
	mpPercentBandButton->setCheckable(true);
	QObject::connect(mpPercentBandButton, &QPushButton::clicked, this, &cHySpexCamera_StatusView::bandButtonToggled);

	mpFocusButton = new QPushButton("Focus");
	mpFocusButton->setCheckable(true);
	QObject::connect(mpFocusButton, &QPushButton::clicked, this, &cHySpexCamera_StatusView::focusButtonToggled);

	mpSpatialDistribution = new QPushButton("Spatial Distribution");
	mpSpatialDistribution->setCheckable(true);
	QObject::connect(mpSpatialDistribution, &QPushButton::clicked, this, &cHySpexCamera_StatusView::SpatialDistributionButtonToggled);

	mpSpectralDistribution = new QPushButton("Spectral Distribution");
	mpSpectralDistribution->setCheckable(true);
	QObject::connect(mpSpectralDistribution, &QPushButton::clicked, this, &cHySpexCamera_StatusView::SpectralDistributionButtonToggled);

	mpDoBackground = new QPushButton("Calculate Background");
	QObject::connect(mpDoBackground, &QPushButton::pressed, this, &cHySpexCamera_StatusView::backgroundPressed);

	mpCurrentFocusLabel = new QLabel("Current Focus");
	mpCurrentFocusLabel->hide();
	mpCurrentFocus = new QLineEdit();
	mpCurrentFocus->setReadOnly(true);
	mpCurrentFocus->hide();

	mpBestFocusLabel = new QLabel("Best Focus");
	mpBestFocusLabel->hide();
	mpBestFocus = new QLineEdit();
	mpBestFocus->setReadOnly(true);
	mpBestFocus->hide();


	mpPlot = new QCustomPlot();
	mpPlot->setMinimumHeight(300);
	mpPlot->addGraph();

	// set axes ranges, so we see all data:
	mpPlot->xAxis->setRange(0, 1);
	mpPlot->yAxis->setRange(0, 1);
	mpPlot->axisRect()->setupFullAxesBox();
}

void cHySpexCamera_StatusView::doStatusLayout(QBoxLayout* pMainLayout)
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

void cHySpexCamera_StatusView::doAcqStatusLayout(QBoxLayout* pMainLayout)
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

void cHySpexCamera_StatusView::doLensInfoLayout(QBoxLayout* pMainLayout)
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

void cHySpexCamera_StatusView::doPlotLayout(QBoxLayout* pMainLayout)
{
	auto* plotLayout = new QVBoxLayout();

	auto* buttonLayout = new QHBoxLayout();

	buttonLayout->addWidget(mpPercentSaturationButton);
	buttonLayout->addWidget(mpPercentBandButton);
	buttonLayout->addWidget(mpFocusButton);
	buttonLayout->addWidget(mpSpatialDistribution);
	buttonLayout->addWidget(mpSpectralDistribution);
	buttonLayout->addStretch();
	buttonLayout->addWidget(mpDoBackground);

	plotLayout->addLayout(buttonLayout);

	auto* focusLayout = new QHBoxLayout();
	focusLayout->addStretch();
	focusLayout->addWidget(mpCurrentFocusLabel);
	focusLayout->addWidget(mpCurrentFocus);
	focusLayout->addWidget(mpBestFocusLabel);
	focusLayout->addWidget(mpBestFocus);
	focusLayout->addStretch();
	plotLayout->addLayout(focusLayout);

	plotLayout->addWidget(mpPlot);

	pMainLayout->addLayout(plotLayout);
}

void cHySpexCamera_StatusView::onInitStatusChange()
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

void cHySpexCamera_StatusView::onAcqStatusChange()
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

void cHySpexCamera_StatusView::onBgStatusChange()
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

void cHySpexCamera_StatusView::onCommStatusChange()
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

void cHySpexCamera_StatusView::onCoolingStatusChange()
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

void cHySpexCamera_StatusView::onShutterStatusChange()
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


void cHySpexCamera_StatusView::onAvgFramesChange(std::uint16_t avgFrames)
{
	mpAvgFrames->setText(QString::number(avgFrames));
}

void cHySpexCamera_StatusView::onFramePeriodChange(std::uint32_t period_us)
{
	mpFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexCamera_StatusView::onMinFramePeriodChange(std::uint32_t period_us)
{
	mpMinFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexCamera_StatusView::onIntegrationTimeChange(std::uint32_t time_us)
{
	mpIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexCamera_StatusView::onMaxIntegrationTimeChange(std::uint32_t time_us)
{
	mpMaxIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexCamera_StatusView::onAmbientTempChange(double temp_C)
{
	mpAmbientTemp_C->setText(QString::number(temp_C, 'f', 1));
}

void cHySpexCamera_StatusView::onSensorTempChange(double temp_C)
{
	QString label = QString::number(temp_C, 'f', 1);
	label += " C";
	mpCoolingStatus->setText(label);
}

void cHySpexCamera_StatusView::onLensInfoChange()
{
	auto name = mpModel->getLensName();
	auto wd_cm = mpModel->getWorkingDistance_cm();
	auto fov_deg = mpModel->getFieldOfView_deg();

	mpLensName->setText(QString::fromStdString(name));
	mpLensWorkingDistance_cm->setText(QString::number(wd_cm, 'f', 1));
	mpLensFieldOfView_deg->setText(QString::number(fov_deg, 'f', 1));
}

void cHySpexCamera_StatusView::onComputeModeChange()
{
	if (mpModel->getComputeState() == cHySpexCameraModel::eCompute::NONE)
	{
		unclickAllButtons(nullptr);
	}
}


void cHySpexCamera_StatusView::saturationButtonToggled(bool state)
{
	if (state)
	{
		unclickAllButtons(mpPercentSaturationButton);

		mpPlot->yAxis->setRange(-1, 100);
		mpPlot->yAxis->setLabel("% Saturation");
		mpPlot->yAxis2->setLabel("Number Saturated");
		mpPlot->yAxis2->setTickLabels(true);

		auto chn = mpModel->getSpatialSize();
		auto bands = mpModel->getSpectralSize();

		mpPlot->xAxis->setRange(0, chn);
		mpPlot->xAxis->setLabel("Spatial Channel");
		mpPlot->graph(0)->data()->clear();
		if (mpPlot->graphCount() == 1)
		{
			auto graph = mpPlot->addGraph(mpPlot->xAxis, mpPlot->yAxis2);
			graph->setPen(QPen(Qt::red));
			mpPlot->yAxis2->setRange(-0.1, 5);
		}
		else
		{
			mpPlot->graph(1)->data()->clear();
		}

		mpPlot->replot();

		mX.resize(chn);
		mY.resize(chn);
		for (std::size_t i = 0; i < chn; ++i)
			mX[i] = i;
	}

	mpModel->computePercentSaturation(state);
}


void cHySpexCamera_StatusView::onSaturationDataUpdated()
{
	static QTime timeStart = QTime::currentTime();
	static int lastPointKey_ms = 0;

	auto key_ms = timeStart.msecsTo(QTime::currentTime());

	// at most add point every MAX_UPDATE_RATE_MSEC
	if ((key_ms - lastPointKey_ms) < MAX_UPDATE_RATE_MSEC)
		return;

	lastPointKey_ms = key_ms;

	auto intensity_pct = mpModel->getMaxIntensity_pct();

	if (mY.size() != intensity_pct.size())
	{
		return;
	}

	for (std::size_t i = 0; i < intensity_pct.size(); ++i)
		mY[i] = intensity_pct[i];

	mpPlot->graph(0)->setData(mX, mY, true);

	auto num_saturated = mpModel->getNumSaturated();

	if (mY.size() != num_saturated.size())
	{
		return;
	}

	for (std::size_t i = 0; i < num_saturated.size(); ++i)
		mY[i] = num_saturated[i];

	mpPlot->graph(1)->setData(mX, mY, true);
	mpPlot->graph(1)->rescaleValueAxis(true);

	mpPlot->replot();
}

void cHySpexCamera_StatusView::bandButtonToggled(bool state)
{
	if (state)
	{
		unclickAllButtons(mpPercentBandButton);

		mpPlot->yAxis->setRange(-1, 100);
		mpPlot->yAxis->setLabel("% Saturation");
		mpPlot->yAxis2->setLabel("Number Saturated");
		mpPlot->yAxis2->setTickLabels(true);

		auto wavelengths_nm = mpModel->getSpectralCalibrationPerBand();

		mpPlot->xAxis->setRange(wavelengths_nm[0], wavelengths_nm[wavelengths_nm.size() - 1]);
		mpPlot->xAxis->setLabel("Wavelengths (nm)");

		mpPlot->graph(0)->data()->clear();
		if (mpPlot->graphCount() == 1)
		{
			auto graph = mpPlot->addGraph(mpPlot->xAxis, mpPlot->yAxis2);
			graph->setPen(QPen(Qt::red));
			mpPlot->yAxis2->setRange(-0.1, 5);
		}
		else
		{
			mpPlot->graph(1)->data()->clear();
		}

		mpPlot->replot();

		mX.resize(wavelengths_nm.size());
		mY.resize(wavelengths_nm.size());
		for (std::size_t i = 0; i < wavelengths_nm.size(); ++i)
			mX[i] = wavelengths_nm[i];
	}

	mpModel->computePercentBand(state);
}

void cHySpexCamera_StatusView::onBandDataUpdated()
{
	static QTime timeStart = QTime::currentTime();
	static int lastPointKey_ms = 0;

	auto key_ms = timeStart.msecsTo(QTime::currentTime());

	// at most add point every 2 ms
	if ((key_ms - lastPointKey_ms) < MAX_UPDATE_RATE_MSEC)
		return;

	lastPointKey_ms = key_ms;

	auto intensity_pct = mpModel->getMaxIntensity_pct();

	if (mY.size() != intensity_pct.size())
	{
		return;
	}

	for (std::size_t i = 0; i < intensity_pct.size(); ++i)
		mY[i] = intensity_pct[i];

	mpPlot->graph(0)->setData(mX, mY, true);

	auto num_saturated = mpModel->getNumSaturated();

	if (mY.size() != num_saturated.size())
	{
		return;
	}

	for (std::size_t i = 0; i < num_saturated.size(); ++i)
		mY[i] = num_saturated[i];

	mpPlot->graph(1)->setData(mX, mY, true);
	mpPlot->graph(1)->rescaleValueAxis(true);

	mpPlot->replot();
}

void cHySpexCamera_StatusView::focusButtonToggled(bool state)
{
	if (state)
	{
		unclickAllButtons(mpFocusButton);

		mpPlot->yAxis->setRange(0, 10.0);
		mpPlot->yAxis->setLabel("Focus");
		mpPlot->yAxis2->setLabel("");
		mpPlot->yAxis2->setTickLabels(false);

		mpPlot->xAxis->setRange(0, 100);
		mpPlot->xAxis->setLabel("Time");
		mpPlot->graph(0)->data()->clear();

		if (mpPlot->graphCount() == 2)
		{
			mpPlot->graph(1)->data()->clear();
			mpPlot->removeGraph(1);
		}

		mpPlot->replot();

		mFocusTimeStart = QTime::currentTime();
		mFocusLastPointKey_ms = 0;
		mMaxFocusValue = 0.0;
		mFocusCounter = 0;
	}

	mpModel->computeFocus(state);
}

void cHySpexCamera_StatusView::onFocusDataUpdated(double focus_number)
{
	auto key_ms = mFocusTimeStart.msecsTo(QTime::currentTime());

	// at most add point every MAX_UPDATE_RATE_MSEC
	if ((key_ms - mFocusLastPointKey_ms) < FOCUS_UPDATE_RATE_MSEC)
		return;

	mFocusLastPointKey_ms = key_ms;
	++mFocusCounter;

	mMaxFocusValue = std::max(mMaxFocusValue, focus_number);

	mpCurrentFocus->setText(QString::number(focus_number, 'f', 1));
	mpBestFocus->setText(QString::number(mMaxFocusValue, 'f', 1));

	// add data to lines:
	mpPlot->graph(0)->addData(mFocusCounter, focus_number);

	// make key axis range scroll with the data:
	mpPlot->xAxis->setRange(mFocusCounter, 100, Qt::AlignRight);
	mpPlot->replot();
}

void cHySpexCamera_StatusView::SpatialDistributionButtonToggled(bool state)
{
	if (state)
	{
		unclickAllButtons(mpSpatialDistribution);

		mpPlot->yAxis->setRange(0, 100);
		QString str = "Digital Number (max = ";
		str += QString::number(mpModel->getMaxPixelValue());
		str += ")";
		mpPlot->yAxis->setLabel(str);
		mpPlot->yAxis2->setLabel("");
		mpPlot->yAxis2->setTickLabels(false);

		auto chn = mpModel->getSpatialSize();

		mpPlot->xAxis->setRange(0, chn);
		mpPlot->xAxis->setLabel("Channel Number");
		mpPlot->graph(0)->data()->clear();

		if (mpPlot->graphCount() == 2)
		{
			mpPlot->graph(1)->data()->clear();
			mpPlot->removeGraph(1);
		}

		mpPlot->replot();

		mX.resize(chn);
		mY.resize(chn);
		for (std::size_t i = 0; i < chn; ++i)
			mX[i] = i;
	}

	mpModel->computeSpatialDistribution(state);
}

void cHySpexCamera_StatusView::onSpatialDistributionUpdated()
{
	static QTime timeStart = QTime::currentTime();
	static int lastPointKey_ms = 0;

	auto key_ms = timeStart.msecsTo(QTime::currentTime());

	// at most add point every 2 ms
	if ((key_ms - lastPointKey_ms) < MAX_UPDATE_RATE_MSEC)
		return;

	lastPointKey_ms = key_ms;

	auto data = mpModel->getSpatialDistributionData();

	float max_y = 0;

	if (mY.size() != data.size())
	{
		return;
	}

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		mY[i] = data[i];
		if (data[i] > max_y) max_y = data[i];
	}

	if (max_y < 5)
		max_y = 5.0f;
	else
		max_y = 10.0f * ((static_cast<int>(max_y) / 10) + 1);

	mpPlot->graph(0)->setData(mX, mY, true);
	mpPlot->yAxis->setRangeUpper(max_y);

	mpPlot->replot();
}

void cHySpexCamera_StatusView::SpectralDistributionButtonToggled(bool state)
{
	if (state)
	{
		unclickAllButtons(mpSpectralDistribution);

		mpPlot->yAxis->setRange(0, 100);
		QString str = "Digital Number (max = ";
		str += QString::number(mpModel->getMaxPixelValue());
		str += ")";
		mpPlot->yAxis->setLabel(str);
		mpPlot->yAxis2->setLabel("");
		mpPlot->yAxis2->setTickLabels(false);

		auto wavelengths_nm = mpModel->getSpectralCalibrationPerBand();

		mpPlot->xAxis->setRange(wavelengths_nm[0], wavelengths_nm[wavelengths_nm.size()-1]);
		mpPlot->xAxis->setLabel("Wavelengths (nm)");
		mpPlot->graph(0)->data()->clear();

		if (mpPlot->graphCount() == 2)
		{
			mpPlot->graph(1)->data()->clear();
			mpPlot->removeGraph(1);
		}

		mX.resize(wavelengths_nm.size());
		mY.resize(wavelengths_nm.size());

		for (std::size_t i = 0; i < wavelengths_nm.size(); ++i)
		{
			mX[i] = wavelengths_nm[i];
			mY[i] = 0.0;
		}

		mpPlot->replot();
	}

	mpModel->computeSpectralDistribution(state);

}

void cHySpexCamera_StatusView::onSpectralDistributionUpdated()
{
	static QTime timeStart = QTime::currentTime();
	static int lastPointKey_ms = 0;

	auto key_ms = timeStart.msecsTo(QTime::currentTime());

	// at most add point every 2 ms
	if ((key_ms - lastPointKey_ms) < MAX_UPDATE_RATE_MSEC)
		return;

	lastPointKey_ms = key_ms;

	auto data = mpModel->getSpectralDistributionData();
	float max_y = 0;

	if (mY.size() != data.size())
	{
		return;
	}

	for (std::size_t i = 0; i < data.size(); ++i)
	{
		mY[i] = data[i];
		if (data[i] > max_y) max_y = data[i];
	}

	if (max_y < 5)
		max_y = 5.0f;
	else
		max_y = 10.0f * ((static_cast<int>(max_y) / 10) + 1);

	mpPlot->graph(0)->setData(mX, mY, true);
	mpPlot->yAxis->setRangeUpper(max_y);

	mpPlot->replot();
}

void cHySpexCamera_StatusView::updateImage(QImage image)
{

}

void cHySpexCamera_StatusView::backgroundPressed()
{
	unclickAllButtons(nullptr);
	mpModel->calcBackground();
}

void cHySpexCamera_StatusView::unclickAllButtons(QPushButton* pExcept)
{
	// Only one button can be active at one time!
	if (pExcept != mpPercentSaturationButton)
		mpPercentSaturationButton->setChecked(false);

	if (pExcept != mpPercentBandButton)
		mpPercentBandButton->setChecked(false);

	if (pExcept != mpFocusButton)
	{
		mpFocusButton->setChecked(false);
		mpCurrentFocusLabel->hide();
		mpCurrentFocus->hide();
		mpBestFocusLabel->hide();
		mpBestFocus->hide();
	}
	else
	{
		mpCurrentFocusLabel->show();
		mpCurrentFocus->show();
		mpBestFocusLabel->show();
		mpBestFocus->show();
	}

	if (pExcept != mpSpatialDistribution)
	{
		mpSpatialDistribution->setChecked(false);
	}
	else
	{
	}

	if (pExcept != mpSpectralDistribution)
	{
		mpSpectralDistribution->setChecked(false);
	}
	else
	{
	}
}
