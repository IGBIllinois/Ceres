
#include "WindSpeedAndDirection.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>


cWindSpeedAndDirection::cWindSpeedAndDirection(QWidget* parent)
	:
	QStatusBar(parent), mpWindSpeed_mph(nullptr), myWindDirection_deg(nullptr)
{
	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance(" XXX.X ");

	mpWindSpeed_mph = new QLineEdit(this);
	mpWindSpeed_mph->setReadOnly(true);
	mpWindSpeed_mph->setFixedWidth(pixelsWide);
	mpWindSpeed_mph->setToolTip(tr("Wind Speed in mph"));

	myWindDirection_deg = new QLineEdit(this);
	myWindDirection_deg->setReadOnly(true);
	myWindDirection_deg->setFixedWidth(pixelsWide);
	myWindDirection_deg->setToolTip(tr("Wind Direction"));

	addPermanentWidget(mpWindSpeed_mph);
	addPermanentWidget(myWindDirection_deg);

	setWindowTitle("Wind Speed and Direction");
}

void cWindSpeedAndDirection::update(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
{
	if (valid_wind_speed)
	{
		mpWindSpeed_mph->setText(QString::number(wind_speed_mps, 'f', 1));
	}
	else
	{
		mpWindSpeed_mph->setText("Calm");
	}

	myWindDirection_deg->setText(QString::number(wind_dir_deg, 'd'));
}
