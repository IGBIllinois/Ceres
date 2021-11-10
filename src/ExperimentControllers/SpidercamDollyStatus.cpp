
#include "SpidercamDollyStatus.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>


cSpidercamDollyStatus::cSpidercamDollyStatus(QWidget* parent)
	: QAbstractScrollArea(parent)
{
	setWindowTitle("Dolly Status");

	createWidgets();

	verticalLayout();
	setFixedSize();
}

cSpidercamDollyStatus::~cSpidercamDollyStatus()
{
}

void cSpidercamDollyStatus::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;
}

void cSpidercamDollyStatus::updatePosition(spidercam::sPosition pos)
{
	repaint();
}

void cSpidercamDollyStatus::updateBatteryLevel(float level_pct)
{
	repaint();
}


void cSpidercamDollyStatus::createWidgets()
{
	mpX_Label = new QLabel();
	mpX_Label->setText("X (m):");
	mpX_m = new QLineEdit();
	mpX_m->setReadOnly(true);

	mpY_Label = new QLabel();
	mpY_Label->setText("Y (m):");
	mpY_m = new QLineEdit();
	mpY_m->setReadOnly(true);

	mpZ_Label = new QLabel();
	mpZ_Label->setText("Z (m):");
	mpZ_m = new QLineEdit();
	mpZ_m->setReadOnly(true);

	mpHeightLabel = new QLabel();
	mpHeightLabel->setText("Height (m):");
	mpHeight_m = new QLineEdit();
	mpHeight_m->setReadOnly(true);

	mpPanLabel = new QLabel();
	mpPanLabel->setText("Pan (deg):");
	mpPan_deg = new QLineEdit();
	mpPan_deg->setReadOnly(true);

	mpTiltLabel = new QLabel();
	mpTiltLabel->setText("Tilt (deg):");
	mpTilt_deg = new QLineEdit();
	mpTilt_deg->setReadOnly(true);

	mpSpeedLabel = new QLabel();
	mpSpeedLabel->setText("Speed (m/s):");
	mpSpeed_mps = new QLineEdit();
	mpSpeed_mps->setReadOnly(true);

	mpBatteryLabel = new QLabel();
	mpBatteryLabel->setText("Battery (pct)");
	mpBatteryLevel_pct = new QLineEdit();
	mpBatteryLevel_pct->setReadOnly(true);

/*
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
*/
}

void cSpidercamDollyStatus::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QGridLayout(this);
	mainlayout->addWidget(mpX_Label, 0, 0);
	mainlayout->addWidget(mpX_m, 0, 1);
	mainlayout->addWidget(mpY_Label, 0, 2);
	mainlayout->addWidget(mpY_m, 0, 3);
	mainlayout->addWidget(mpZ_Label, 0, 4);
	mainlayout->addWidget(mpZ_m, 0, 5);
	mainlayout->addWidget(mpHeightLabel, 0, 6);
	mainlayout->addWidget(mpHeight_m, 0, 7);

	mainlayout->addWidget(mpPanLabel, 1, 0);
	mainlayout->addWidget(mpPan_deg, 1, 1);
	mainlayout->addWidget(mpTiltLabel, 1, 2);
	mainlayout->addWidget(mpTilt_deg, 1, 3);
	mainlayout->addWidget(mpSpeedLabel, 1, 4);
	mainlayout->addWidget(mpSpeed_mps, 1, 5);
	mainlayout->addWidget(mpBatteryLabel, 1, 6);
	mainlayout->addWidget(mpBatteryLevel_pct, 1, 7);
/*
	mainlayout->addWidget(mpTimestampLabel, 2, 4);
	mainlayout->addWidget(mpTimestamp_s, 2, 5);
	mainlayout->addWidget(mpDateLabel, 3, 0);
	mainlayout->addWidget(mpDate, 3, 1);
	mainlayout->addWidget(mpTimeLabel, 3, 2);
	mainlayout->addWidget(mpTime, 3, 3);
*/
	mainlayout->setSpacing(5);
	setLayout(mainlayout);
}

void cSpidercamDollyStatus::verticalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QFormLayout(this);
	mainlayout->addRow(mpX_Label, mpX_m);
	mainlayout->addRow(mpY_Label, mpY_m);
	mainlayout->addRow(mpZ_Label, mpZ_m);
	mainlayout->addRow(mpHeightLabel, mpHeight_m);
	mainlayout->addRow(mpPanLabel, mpPan_deg);
	mainlayout->addRow(mpTiltLabel, mpTilt_deg);
	mainlayout->addRow(mpSpeedLabel, mpSpeed_mps);
	mainlayout->addRow(mpBatteryLabel, mpBatteryLevel_pct);
	
/*
	mainlayout->addRow(mpTimestampLabel, mpTimestamp_s);
	mainlayout->addRow(mpDateLabel, mpDate);
	mainlayout->addRow(mpTimeLabel, mpTime);
*/

	setLayout(mainlayout);
}

void cSpidercamDollyStatus::setFixedSize()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//	setMinimumSize(687, 139);
//	setMaximumSize(687, 139);
}

void cSpidercamDollyStatus::setVariableSize()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	setMinimumSize(0, 0);
//	setMaximumSize(16777215, 16777215);
}

void cSpidercamDollyStatus::dockLocationChanged(Qt::DockWidgetArea area)
{
	setVariableSize();

	if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea))
	{
		verticalLayout();
		return;
	}

	if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea))
	{
		horizontalLayout();
		return;
	}
}

void cSpidercamDollyStatus::topLevelChanged(bool topLevel)
{
	// topLevel is true when our GPS view is floating. 
	if (topLevel)
	{
		verticalLayout();
		setFixedSize();
	}
}
