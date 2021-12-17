
#include "WindSpeedAndDirection.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>


cWindSpeedAndDirection::cWindSpeedAndDirection(QWidget* parent)
	:
	QStatusBar(parent), mpWindSpeed_mph(nullptr), myWindDirection_deg(nullptr)
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
	myWindDirection_deg = new QLineEdit(this);
	myWindDirection_deg->setReadOnly(true);
	myWindDirection_deg->setFixedWidth(pixelsWide);
	myWindDirection_deg->setAlignment(Qt::AlignCenter);
	myWindDirection_deg->setToolTip(tr("Wind Direction in degrees"));

	addPermanentWidget(mpWindSpeed_mph);
	addPermanentWidget(myWindDirection_deg);

	setWindowTitle("Wind Speed and Direction");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void cWindSpeedAndDirection::updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg)
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
	s += QString::number(static_cast<int>(wind_dir_deg));
	myWindDirection_deg->setText(s);
}
