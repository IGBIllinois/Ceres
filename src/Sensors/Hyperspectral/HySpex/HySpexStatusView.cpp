
#include "HySpexStatusView.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cHySpexStatusView::cHySpexStatusView(cSensorModel* pModel, QWidget* parent)
:
    cSensorStatusView(pModel, parent)
{
}

cHySpexStatusView::~cHySpexStatusView()
{
}

void cHySpexStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpAcquisitionStatus = new QButtonIndicator(this);
	mpAcquisitionStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpAcquisitionStatus->setMinimumWidth(50);

	mpBackgroundStatus = new QButtonIndicator(this);
	mpBackgroundStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpBackgroundStatus->setMinimumWidth(50);

	mpCommunicationStatus = new QButtonIndicator(this);
	mpCommunicationStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpCommunicationStatus->setMinimumWidth(50);

	mpCoolingStatus = new QButtonIndicator(this);
	mpCoolingStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpCoolingStatus->setMinimumWidth(50);

	mpShutterStatus = new QButtonIndicator(this);
	mpShutterStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mpShutterStatus->setMinimumWidth(50);

//	mpTest->setState(QButtonIndicator::ACTIVE);

//	auto font = mpTest->font();
//	font.setPointSize(12);
//	mpTest->setFont(font);
}

void cHySpexStatusView::doSubLayout(QBoxLayout* pMainLayout)
{
	QGroupBox* cameraStatusBox = new QGroupBox("Camera Status");

	auto* statusLayout = new QHBoxLayout();

	statusLayout->addWidget(mpCommunicationStatus);
	statusLayout->addWidget(mpAcquisitionStatus);
	statusLayout->addWidget(mpCoolingStatus);
	statusLayout->addWidget(mpBackgroundStatus);
	statusLayout->addWidget(mpShutterStatus);

	cameraStatusBox->setLayout(statusLayout);

	pMainLayout->addWidget(cameraStatusBox);

}

void cHySpexStatusView::onAcqStatusChange(hyspex::AcquisitionStatus status)
{
	using namespace hyspex;

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

void cHySpexStatusView::onBgStatusChange(hyspex::BackgroundStatus status)
{
	using namespace hyspex;

	switch (status)
	{
	case HYSPEX_BG_INVALID:
		mpBackgroundStatus->setState(QButtonIndicator::UNKNOWN, "BG NONE");
		break;
	case HYSPEX_BG_PENDING:
		mpBackgroundStatus->setState(QButtonIndicator::WARNING, "BG PENDING");
		break;
	case HYSPEX_BG_VALID:
	case HYSPEX_BG_PENDING_READY:
		mpBackgroundStatus->setState(QButtonIndicator::OK, "BG OK");
		break;
	case HYSPEX_BG_EXPIRED:
		mpBackgroundStatus->setState(QButtonIndicator::ALERT, "BG EXPIRED");
		break;
	case HYSPEX_BG_ABORTED:
		mpBackgroundStatus->setState(QButtonIndicator::ERROR, "BG ABORTED");
		break;
	}
}

void cHySpexStatusView::onCommStatusChange(hyspex::CommunicationStatus status)
{
	using namespace hyspex;

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

void cHySpexStatusView::onCoolingStatusChange(hyspex::CoolingStatus status)
{
	using namespace hyspex;

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

void cHySpexStatusView::onShutterStatusChange(hyspex::ShutterStatus status)
{
	using namespace hyspex;

	switch (status)
	{
	case HYSPEX_SHUTTER_OPEN:
		mpShutterStatus->setState(QButtonIndicator::OK, "SH OPEN");
		break;
	case HYSPEX_SHUTTER_CLOSED:
		mpShutterStatus->setState(QButtonIndicator::OK, "SH CLOSE");
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
		mpShutterStatus->setState(QButtonIndicator::UNKNOWN, "SH UNKN");
		break;
	}
}



