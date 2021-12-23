
#include "GpsView.hpp"
#include "../../Utilities/Constants.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cGpsView::cGpsView(QWidget* parent)
	:
	QAbstractScrollArea(parent)
{
	createWidgets();
}

cGpsView::~cGpsView()
{

}

void cGpsView::createWidgets()
{
	mpLatitudeLabel = new QLabel();
	mpLatitudeLabel->setText("Latitude (deg):");
	mpLatitude_deg = new QLineEdit();
	mpLatitude_deg->setReadOnly(true);

	mpLongitudeLabel = new QLabel();
	mpLongitudeLabel->setText("Longitude (deg):");
	mpLongitude_deg = new QLineEdit();
	mpLongitude_deg->setReadOnly(true);

	mpHeightLabel = new QLabel();
	mpHeightLabel->setText("Height (m):");
	mpHeight_m = new QLineEdit();
	mpHeight_m->setReadOnly(true);

	mpNorthSpeedLabel = new QLabel();
	mpNorthSpeedLabel->setText("North Speed (m/s):");
	mpNorthVelocity_mps = new QLineEdit();
	mpNorthVelocity_mps->setReadOnly(true);

	mpEastSpeedLabel = new QLabel();
	mpEastSpeedLabel->setText("East Speed (m/s):");
	mpEastVelocity_mps = new QLineEdit();
	mpEastVelocity_mps->setReadOnly(true);

	mpVerticalSpeedLabel = new QLabel();
	mpVerticalSpeedLabel->setText("Vertical Speed (m/s):");
	mpUpVelocity_mps = new QLineEdit();
	mpUpVelocity_mps->setReadOnly(true);

	mpGroundTrackLabel = new QLabel();
	mpGroundTrackLabel->setText("Ground Track (deg):");
	mpGroundTrack_deg = new QLineEdit();
	mpGroundTrack_deg->setReadOnly(true);

	mpDatumLabel = new QLabel();
	mpDatumLabel->setText("Datum:");
	mpDatum = new QLineEdit();
	mpDatum->setReadOnly(true);

	mpTimestampLabel = new QLabel();
	mpTimestampLabel->setText("Timestamp (s):");
	mpTimestamp_s = new QLineEdit();
	mpTimestamp_s->setReadOnly(true);

	mpDateLabel = new QLabel();
	mpDateLabel->setText("Date:");
	mpDate = new QLineEdit();
	mpDate->setReadOnly(true);

	mpTimeLabel = new QLabel();
	mpTimeLabel->setText("Time:");
	mpTime = new QLineEdit();
	mpTime->setReadOnly(true);
}

void cGpsView::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QGridLayout(this);
	mainlayout->addWidget(mpLatitudeLabel, 0, 0);
	mainlayout->addWidget(mpLatitude_deg, 0, 1);
	mainlayout->addWidget(mpLongitudeLabel, 0, 2);
	mainlayout->addWidget(mpLongitude_deg, 0, 3);
	mainlayout->addWidget(mpHeightLabel, 0, 4);
	mainlayout->addWidget(mpHeight_m, 0, 5);
	mainlayout->addWidget(mpNorthSpeedLabel, 1, 0);
	mainlayout->addWidget(mpNorthVelocity_mps, 1, 1);
	mainlayout->addWidget(mpEastSpeedLabel, 1, 2);
	mainlayout->addWidget(mpEastVelocity_mps, 1, 3);
	mainlayout->addWidget(mpVerticalSpeedLabel, 1, 4);
	mainlayout->addWidget(mpUpVelocity_mps, 1, 5);
	mainlayout->addWidget(mpGroundTrackLabel, 2, 0);
	mainlayout->addWidget(mpGroundTrack_deg, 2, 1);
	mainlayout->addWidget(mpDatumLabel, 2, 2);
	mainlayout->addWidget(mpDatum, 2, 3);
	mainlayout->addWidget(mpTimestampLabel, 2, 4);
	mainlayout->addWidget(mpTimestamp_s, 2, 5);
	mainlayout->addWidget(mpDateLabel, 3, 0);
	mainlayout->addWidget(mpDate, 3, 1);
	mainlayout->addWidget(mpTimeLabel, 3, 2);
	mainlayout->addWidget(mpTime, 3, 3);

	setLayout(mainlayout);
}

void cGpsView::verticalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QFormLayout(this);
	mainlayout->addRow(mpLatitudeLabel, mpLatitude_deg);
	mainlayout->addRow(mpLongitudeLabel, mpLongitude_deg);
	mainlayout->addRow(mpHeightLabel, mpHeight_m);
	mainlayout->addRow(mpNorthSpeedLabel, mpNorthVelocity_mps);
	mainlayout->addRow(mpEastSpeedLabel, mpEastVelocity_mps);
	mainlayout->addRow(mpVerticalSpeedLabel, mpUpVelocity_mps);
	mainlayout->addRow(mpGroundTrackLabel, mpGroundTrack_deg);
	mainlayout->addRow(mpDatumLabel, mpDatum);
	mainlayout->addRow(mpTimestampLabel, mpTimestamp_s);
	mainlayout->addRow(mpDateLabel, mpDate);
	mainlayout->addRow(mpTimeLabel, mpTime);

	setLayout(mainlayout);
}

void cGpsView::setFloatingSize()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMinimumSize(687, 139);
	setMaximumSize(687, 139);
}

void cGpsView::setHorzDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	setMinimumSize(687, 139);
	setMaximumSize(16777215, 139);
}

void cGpsView::setVertDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(250, 350);
	setMaximumSize(16777215, 16777215);
}

void cGpsView::dockLocationChanged(Qt::DockWidgetArea area)
{
	if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea))
	{
		verticalLayout();
		setVertDockSize();
		return;
	}

	if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea))
	{
		horizontalLayout();
		setHorzDockSize();
		return;
	}
}

void cGpsView::topLevelChanged(bool topLevel)
{
	// topLevel is true when our GPS view is floating. 
	if (topLevel)
	{
		horizontalLayout();
		setFloatingSize();
	}
}
