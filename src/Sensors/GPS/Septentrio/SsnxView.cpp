
#include "SsnxView.hpp"
#include "Constants.hpp"

#include "../GpsUtils.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cSsnxView::cSsnxView(QWidget* parent)
	:
	cGpsView(parent)
{
	setWindowTitle("SSNX GPS");
}

cSsnxView::~cSsnxView()
{

}

void cSsnxView::updateSolutionType(int solution_type)
{
	mpSolutionType->setText(::gps::to_qstring(::gps::to_solution_type(solution_type)));
}

void cSsnxView::updatePVT(double timestamp_s,
	double lat_rad, double lng_rad, double height_m,
	double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
	double groundTrack_deg, int datum, int num_sv, int num_bases)
{
	mpLatitude_deg->setText(QString::number(lat_rad * nConstants::RAD_TO_DEG, 'f', 10));
	mpLongitude_deg->setText(QString::number(lng_rad * nConstants::RAD_TO_DEG, 'f', 10));
	mpHeight_m->setText(QString::number(height_m));
	mpNorthVelocity_mps->setText(QString::number(northSpeed_mps));
	mpEastVelocity_mps->setText(QString::number(eastSpeed_mps));
	mpUpVelocity_mps->setText(QString::number(vertSpeed_mps));
	mpGroundTrack_deg->setText(QString::number(groundTrack_deg));
	mpTimestamp_s->setText(QString::number(timestamp_s, 'f', 6));
	mpDatum->setText(::gps::to_qstring(::gps::to_datum(datum)));
	mpNumSv->setText(QString::number(num_sv));
	mpNumBases->setText(QString::number(num_bases));
}


void cSsnxView::updateUTC(int hour, int min, int sec, int day, int month, int year)
{
	mpDate->setText(QString("%1/%2/%3").arg(QString::number(month), 2, '0')
		.arg(QString::number(day), 2, '0').arg(QString::number(year)));

	mpTime->setText(QString("%1:%2:%3").arg(QString::number(hour), 2, '0')
		.arg(QString::number(min), 2, '0').arg(QString::number(sec), 2, '0'));
}

