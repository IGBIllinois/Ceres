
#include "AxisCommunicationsStatusView.hpp"
#include "../Sensors/SensorModel.hpp"
#include "Constants.hpp"

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cAxisCommunicationsStatusView::cAxisCommunicationsStatusView(QWidget* parent)
:
	cSensorStatusView(parent)
{
	setWindowTitle("Axis F44 Webcam");
}

cAxisCommunicationsStatusView::~cAxisCommunicationsStatusView()
{

}


void cAxisCommunicationsStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpPvtCartesianValid = new QIndicator(this);
	mpPvtCartesianValid->setText("Cartesian Position/Velocity/Time Valid");

	mpPvtGeodeticValid = new QIndicator(this);
	mpPvtGeodeticValid->setText("Geodetic Position/Velocity/Time Valid");

	mpPosCovGeodeticValid = new QIndicator(this);
	mpPosCovGeodeticValid->setText("Geodetic Position Covariance Matrix Valid");

	mpVelCovGeodeticValid = new QIndicator(this);
	mpVelCovGeodeticValid->setText("Geodetic Velocity Covariance Matrix Valid");

	mpPosProjectedValid = new QIndicator(this);
	mpPosProjectedValid->setText("Projected Position Valid");

	mpReceiverTimeValid = new QIndicator(this);
	mpReceiverTimeValid->setText("Receiver Time Valid");

	mpRtcmDatumValid = new QIndicator(this);
	mpRtcmDatumValid->setText("RTCM Datum Valid");
}

void cAxisCommunicationsStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* packetBox = new QGroupBox("Received Packets");

	auto* packetInfoLayout = new QFormLayout();
	packetInfoLayout->addRow(mpPvtCartesianValid, mpPvtGeodeticValid);
	packetInfoLayout->addRow(mpPosCovGeodeticValid, mpVelCovGeodeticValid);
	packetInfoLayout->addRow(mpPosProjectedValid, mpReceiverTimeValid);
	packetInfoLayout->addRow(mpRtcmDatumValid);

	packetBox->setLayout(packetInfoLayout);

	mainLayout->addWidget(packetBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cAxisCommunicationsStatusView::onPvtCartesianStateChange(bool valid)
{
	mpPvtCartesianValid->setState(valid);
}

void cAxisCommunicationsStatusView::onPvtGeodeticStateChange(bool valid)
{
	mpPvtGeodeticValid->setState(valid);
}

void cAxisCommunicationsStatusView::onPosCovGeodeticStateChange(bool valid)
{
	mpPosCovGeodeticValid->setState(valid);
}

void cAxisCommunicationsStatusView::onVelCovGeodeticStateChange(bool valid)
{
	mpVelCovGeodeticValid->setState(valid);
}

void cAxisCommunicationsStatusView::onPosProjectedStateChange(bool valid)
{
	mpPosProjectedValid->setState(valid);
}

void cAxisCommunicationsStatusView::onReceiverTimeStateChange(bool valid)
{
	mpReceiverTimeValid->setState(valid);
}

void cAxisCommunicationsStatusView::onRtcmDatumStateChange(bool valid)
{
	mpRtcmDatumValid->setState(valid);
}

