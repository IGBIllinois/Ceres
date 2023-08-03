
#include "WindTempRhPAR_StatusBar.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>


cWindTempRhPAR_StatusBar::cWindTempRhPAR_StatusBar(QWidget* parent)
	:
	QStatusBar(parent), mpWindSpeed_mph(nullptr), mpWindDirection_deg(nullptr)
{
	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance("Speed: XXX.X ");

	mpWindSpeed_mph = new QLineEdit(this);
	mpWindSpeed_mph->setReadOnly(true);
	mpWindSpeed_mph->setFixedWidth(pixelsWide);
	mpWindSpeed_mph->setAlignment(Qt::AlignCenter);
	mpWindSpeed_mph->setToolTip(tr("Wind Speed in mph"));

	pixelsWide = fm.horizontalAdvance("Dir: XXX.X ");
	mpWindDirection_deg = new QLineEdit(this);
	mpWindDirection_deg->setReadOnly(true);
	mpWindDirection_deg->setFixedWidth(pixelsWide);
	mpWindDirection_deg->setAlignment(Qt::AlignCenter);
	mpWindDirection_deg->setToolTip(tr("Wind Direction in degrees"));

	pixelsWide = fm.horizontalAdvance("T: XX.X ");
	mpTemperature_C = new QLineEdit(this);
	mpTemperature_C->setReadOnly(true);
	mpTemperature_C->setFixedWidth(pixelsWide);
	mpTemperature_C->setAlignment(Qt::AlignCenter);
	mpTemperature_C->setToolTip(tr("Temperature in Centigrade"));

	pixelsWide = fm.horizontalAdvance("Rh: XXX.X ");
	mpRelativeHumidity_per = new QLineEdit(this);
	mpRelativeHumidity_per->setReadOnly(true);
	mpRelativeHumidity_per->setFixedWidth(pixelsWide);
	mpRelativeHumidity_per->setAlignment(Qt::AlignCenter);
	mpRelativeHumidity_per->setToolTip(tr("Relative Humidity"));

	pixelsWide = fm.horizontalAdvance("PAR: XXX.X ");
	mpPAR_umole = new QLineEdit(this);
	mpPAR_umole->setReadOnly(true);
	mpPAR_umole->setFixedWidth(pixelsWide);
	mpPAR_umole->setAlignment(Qt::AlignCenter);
	mpPAR_umole->setToolTip(tr("PAR in umole/s/m^2"));

	addPermanentWidget(mpWindSpeed_mph);
	addPermanentWidget(mpWindDirection_deg);
	addPermanentWidget(mpTemperature_C);
	addPermanentWidget(mpRelativeHumidity_per);
	addPermanentWidget(mpPAR_umole);

	setWindowTitle("Wind, Temp, Rh, and PAR");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void cWindTempRhPAR_StatusBar::updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
{
	if (valid_wind_speed)
	{
		QString s = "Speed: ";
		s += QString::number(wind_speed_mps * nConstants::MPS_TO_MPH, 'f', 1);
		mpWindSpeed_mph->setText(s);
	}
	else
	{
		mpWindSpeed_mph->setText("Calm");
	}

	QString s = "Dir: ";

	QString dir = QString::number(static_cast<int>(wind_dir_deg));

	if (wind_dir_deg < 10.0)
		dir.insert(0, "00");
	else if (wind_dir_deg < 100.0)
		dir.insert(0, "0");

	s += dir;

	mpWindDirection_deg->setText(s);
}

void cWindTempRhPAR_StatusBar::updateTemperature(double temp_C)
{
	QString s = "T: ";
	s += QString::number(temp_C, 'f', 1);
	mpTemperature_C->setText(s);
}

void cWindTempRhPAR_StatusBar::updateRelativeHumidity(double RH_percent)
{
	QString s = "Rh: ";
	s += QString::number(RH_percent, 'f', 1);
	mpRelativeHumidity_per->setText(s);
}

void cWindTempRhPAR_StatusBar::updatePAR(double par_umole)
{
	QString s = "PAR: ";
	s += QString::number(par_umole, 'f', 1);
	mpPAR_umole->setText(s);
}

