
#include "SsnxStatusView.hpp"
#include "../Sensors/SensorModel.hpp"
#include "Constants.hpp"

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>

namespace
{
/*
	QString to_qstring(gps::eDatum datum)
	{
		switch (datum)
		{
		case gps::eDatum::WGS84: return QString("WGS84");
		case gps::eDatum::DGNSS: return QString("DGNSS");
		case gps::eDatum::ETRS89: return QString("ETRS89");
		case gps::eDatum::NAD83: return QString("NAD83");
		case gps::eDatum::NAD83_PA11: return QString("NAD83 PA11");
		case gps::eDatum::NAD83_MA11: return QString("NAD83 (MA11)");
		case gps::eDatum::GDA94: return QString("GDA94");
		case gps::eDatum::FIRST_USER_DATUM: return QString("First Uset Datum");
		case gps::eDatum::SECOND_USER_DATUM: return QString("Second Uset Datum");
		}

		return QString("Unknown");
	}
*/
}


cSsnxStatusView::cSsnxStatusView(QWidget* parent)
:
	cSensorStatusView(parent)
{
	setWindowTitle("SSNX GPS");
}

cSsnxStatusView::~cSsnxStatusView()
{

}


void cSsnxStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpPvtCartesianValid = new QIndicator("Cartesian Position/Velocity/Time Valid", this);
	mpPvtGeodeticValid  = new QIndicator("Geodetic Position/Velocity/Time Valid", this);

	mpPosCovGeodeticValid = new QIndicator("Geodetic Position Covariance Matrix Valid", this);
	mpVelCovGeodeticValid = new QIndicator("Geodetic Velocity Covariance Matrix Valid", this);

	mpPosProjectedValid = new QIndicator("Projected Position Valid", this);
	mpReceiverTimeValid = new QIndicator("Receiver Time Valid", this);

	mpRtcmDatumValid = new QIndicator("RTCM Datum Valid", this);
}

void cSsnxStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* packetBox = new QGroupBox("Received Packets");

	auto* packetInfoLayout = new QGridLayout();
	packetInfoLayout->addWidget(mpPvtCartesianValid, 0, 0);
	packetInfoLayout->addWidget(mpPvtGeodeticValid, 0, 1);
	packetInfoLayout->addWidget(mpPosCovGeodeticValid, 0, 2);
	packetInfoLayout->addWidget(mpVelCovGeodeticValid, 0, 3);
	packetInfoLayout->addWidget(mpPosProjectedValid, 1, 0);
	packetInfoLayout->addWidget(mpReceiverTimeValid, 1, 1);
	packetInfoLayout->addWidget(mpRtcmDatumValid, 1, 2);

	packetBox->setLayout(packetInfoLayout);

	mainLayout->addWidget(packetBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cSsnxStatusView::onPvtCartesianStateChange(bool valid)
{
	mpPvtCartesianValid->setState(valid);
}

void cSsnxStatusView::onPvtGeodeticStateChange(bool valid)
{
	mpPvtGeodeticValid->setState(valid);
}

void cSsnxStatusView::onPosCovGeodeticStateChange(bool valid)
{
	mpPosCovGeodeticValid->setState(valid);
}

void cSsnxStatusView::onVelCovGeodeticStateChange(bool valid)
{
	mpVelCovGeodeticValid->setState(valid);
}

void cSsnxStatusView::onPosProjectedStateChange(bool valid)
{
	mpPosProjectedValid->setState(valid);
}

void cSsnxStatusView::onReceiverTimeStateChange(bool valid)
{
	mpReceiverTimeValid->setState(valid);
}

void cSsnxStatusView::onRtcmDatumStateChange(bool valid)
{
	mpRtcmDatumValid->setState(valid);
}

