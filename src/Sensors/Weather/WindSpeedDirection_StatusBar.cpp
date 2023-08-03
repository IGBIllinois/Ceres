
#include "WindSpeedDirection_StatusBar.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>


cWindSpeedDirection_StatusBar::cWindSpeedDirection_StatusBar(QWidget* parent)
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

	addPermanentWidget(mpWindSpeed_mph);
	addPermanentWidget(mpWindDirection_deg);

	setWindowTitle("Wind Speed and Direction");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void cWindSpeedDirection_StatusBar::updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
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
