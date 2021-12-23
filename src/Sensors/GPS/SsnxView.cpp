
#include "SsnxView.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>

#include <string>

namespace
{
	QString to_qstring(cGpsModel::eDatum datum)
	{
		switch (datum)
		{
		case cGpsModel::eDatum::WGS84: return QString("WGS84");
		case cGpsModel::eDatum::DGNSS: return QString("DGNSS");
		case cGpsModel::eDatum::ETRS89: return QString("ETRS89");
		case cGpsModel::eDatum::NAD83: return QString("NAD83");
		case cGpsModel::eDatum::NAD83_PA11: return QString("NAD83 PA11");
		case cGpsModel::eDatum::NAD83_MA11: return QString("NAD83 (MA11)");
		case cGpsModel::eDatum::GDA94: return QString("GDA94");
		case cGpsModel::eDatum::FIRST_USER_DATUM: return QString("First Uset Datum");
		case cGpsModel::eDatum::SECOND_USER_DATUM: return QString("Second Uset Datum");
		}

		return QString("Unknown");
	}
}


cSsnxView::cSsnxView(QWidget* parent)
	:
	cGpsView(parent)
{
	setWindowTitle("SSNX GPS");
}

cSsnxView::~cSsnxView()
{

}

void cSsnxView::updatePVT(double timestamp_s,
	double lat_rad, double lng_rad, double height_m,
	double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
	double groundTrack_deg, cGpsModel::eDatum datum)
{
	mpLatitude_deg->setText(QString::number(lat_rad * nConstants::RAD_TO_DEG));
	mpLongitude_deg->setText(QString::number(lng_rad * nConstants::RAD_TO_DEG));
	mpHeight_m->setText(QString::number(height_m));
	mpNorthVelocity_mps->setText(QString::number(northSpeed_mps));
	mpEastVelocity_mps->setText(QString::number(eastSpeed_mps));
	mpUpVelocity_mps->setText(QString::number(vertSpeed_mps));
	mpGroundTrack_deg->setText(QString::number(groundTrack_deg));
	mpTimestamp_s->setText(QString::number(timestamp_s, 'f', 6));
	mpDatum->setText(to_qstring(datum));
}


void cSsnxView::updateUTC(int hour, int min, int sec, int day, int month, int year)
{
	mpDate->setText(QString("%1/%2/%3").arg(QString::number(month), 2, '0')
		.arg(QString::number(day), 2, '0').arg(QString::number(year)));

	mpTime->setText(QString("%1:%2:%3").arg(QString::number(hour), 2, '0')
		.arg(QString::number(min), 2, '0').arg(QString::number(sec), 2, '0'));
}

