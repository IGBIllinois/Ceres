
#include "SsnxStatusView.hpp"
#include "SsnxModel.hpp"
#include "../Sensors/SensorModel.hpp"
#include "Constants.hpp"
#include "QIndicator.hpp"

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


cSsnxStatusView::cSsnxStatusView(cSsnxModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{
}

cSsnxStatusView::~cSsnxStatusView()
{
}


void cSsnxStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpPvtCartesianValid = new QLedIndicator("Cartesian Position/Velocity/Time Valid", this);
	mpPvtGeodeticValid  = new QLedIndicator("Geodetic Position/Velocity/Time Valid", this);

	mpPosCovGeodeticValid = new QLedIndicator("Geodetic Position Covariance Matrix Valid", this);
	mpVelCovGeodeticValid = new QLedIndicator("Geodetic Velocity Covariance Matrix Valid", this);

	mpPosProjectedValid = new QLedIndicator("Projected Position Valid", this);
	mpReceiverTimeValid = new QLedIndicator("Receiver Time Valid", this);

	mpRtcmDatumValid = new QLedIndicator("RTCM Datum Valid", this);

	mpX_mm = new QLineEdit(this);
	mpX_mm->setReadOnly(true);

	mpY_mm = new QLineEdit(this);
	mpY_mm->setReadOnly(true);

	mpZ_mm = new QLineEdit(this);
	mpZ_mm->setReadOnly(true);
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

	mainLayout->addSpacing(10);

	QGroupBox* posBox = new QGroupBox("Approximate Position");

	auto* pPosLayout = new QHBoxLayout();
	QLabel* text = new QLabel("X (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpX_mm, 1);
	pPosLayout->addSpacing(10);

	text = new QLabel("Y (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpY_mm, 1);
	pPosLayout->addSpacing(10);

	text = new QLabel("Z (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpZ_mm, 1);

	posBox->setLayout(pPosLayout);

	mainLayout->addWidget(posBox);

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

void cSsnxStatusView::onPositionChange(int x_mm, int y_mm, int z_mm)
{
	if (x_mm < 0)
		mpX_mm->setText("");
	else
		mpX_mm->setText(QString::number(x_mm));

	if (y_mm < 0)
		mpY_mm->setText("");
	else
		mpY_mm->setText(QString::number(y_mm));

	if (z_mm < 0)
		mpZ_mm->setText("");
	else
		mpZ_mm->setText(QString::number(z_mm));

}
