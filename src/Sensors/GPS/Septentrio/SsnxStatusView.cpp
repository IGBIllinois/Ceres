
#include "SsnxStatusView.hpp"
#include "../Sensors/SensorModel.hpp"
#include "Constants.hpp"

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
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

	mpPvtCartesianValid = new QCheckBox(this);
	mpPvtCartesianValid->setCheckable(false);
	mpPvtCartesianValid->setText("Cartesian Position/Velocity/Time Valid");

	mpPvtGeodeticValid = new QCheckBox(this);
	mpPvtGeodeticValid->setCheckable(false);
	mpPvtGeodeticValid->setText("Geodetic Position/Velocity/Time Valid");

	mpPosCovGeodeticValid = new QCheckBox(this);
	mpPosCovGeodeticValid->setCheckable(false);
	mpPosCovGeodeticValid->setText("Geodetic Position Covariance Matrix Valid");

	mpVelCovGeodeticValid = new QCheckBox(this);
	mpVelCovGeodeticValid->setCheckable(false);
	mpVelCovGeodeticValid->setText("Geodetic Velocity Covariance Matrix Valid");

	mpPosProjectedValid = new QCheckBox(this);
	mpPosProjectedValid->setCheckable(false);
	mpPosProjectedValid->setText("Projected Position Valid");

	mpReceiverTimeValid = new QCheckBox(this);
	mpReceiverTimeValid->setCheckable(false);
	mpReceiverTimeValid->setText("Receiver Time Valid");

	mpRtcmDatumValid = new QCheckBox(this);
	mpRtcmDatumValid->setCheckable(false);
	mpRtcmDatumValid->setText("RTCM Datum Valid");
}

void cSsnxStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	auto* statusLayout = new QHBoxLayout(this);
	statusLayout->addWidget(mpSensorLabel);
	statusLayout->addWidget(mpSensorStatus);
	mainLayout->addLayout(statusLayout);

	mainLayout->addWidget(mpPvtCartesianValid);
	mainLayout->addWidget(mpPvtGeodeticValid);
	mainLayout->addWidget(mpPosCovGeodeticValid);
	mainLayout->addWidget(mpVelCovGeodeticValid);
	mainLayout->addWidget(mpPosProjectedValid);
	mainLayout->addWidget(mpReceiverTimeValid);
	mainLayout->addWidget(mpRtcmDatumValid);

	setLayout(mainLayout);
}

void cSsnxStatusView::onPvtCartesianStateChange(bool valid)
{
	mpPvtCartesianValid->setChecked(valid);
}

void cSsnxStatusView::onPvtGeodeticStateChange(bool valid)
{
	mpPvtGeodeticValid->setChecked(valid);
}

void cSsnxStatusView::onPosCovGeodeticStateChange(bool valid)
{
	mpPosCovGeodeticValid->setChecked(valid);
}

void cSsnxStatusView::onVelCovGeodeticStateChange(bool valid)
{
	mpVelCovGeodeticValid->setChecked(valid);
}

void cSsnxStatusView::onPosProjectedStateChange(bool valid)
{
	mpPosProjectedValid->setChecked(valid);
}

void cSsnxStatusView::onReceiverTimeStateChange(bool valid)
{
	mpReceiverTimeValid->setChecked(valid);
}

void cSsnxStatusView::onRtcmDatumStateChange(bool valid)
{
	mpRtcmDatumValid->setChecked(valid);
}

