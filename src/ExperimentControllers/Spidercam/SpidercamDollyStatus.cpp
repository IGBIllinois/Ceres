
#include "SpidercamDollyStatus.hpp"
#include "../../Utilities/Constants.hpp"

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
	setVertDockSize();
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

void cSpidercamDollyStatus::updatePosition(spidercam::sPosition_1_t pos)
{

	mpX_m->setText(QString::number(pos.X_mm * nConstants::MM_TO_M, 'f', 3));
	mpY_m->setText(QString::number(pos.Y_mm * nConstants::MM_TO_M, 'f', 3));
	mpZ_m->setText(QString::number(pos.Z_mm * nConstants::MM_TO_M, 'f', 3));
	mpHeight_m->setText(QString::number(pos.height_mm * nConstants::MM_TO_M, 'f', 3));

	mpPan_deg->setText(QString::number(pos.pan_deg, 'f', 2));
	mpTilt_deg->setText(QString::number(pos.tilt_deg, 'f', 2));

	mpSpeed_mps->setText(QString::number(pos.speed_mmps * nConstants::MM_TO_M, 'f', 3));
}

void cSpidercamDollyStatus::updateBatteryLevel(float level_pct)
{
	mpBatteryLevel_pct->setText(QString::number(level_pct, 'f', 2));
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
	mpBatteryLabel->setText("Battery (%)");
	mpBatteryLevel_pct = new QLineEdit();
	mpBatteryLevel_pct->setReadOnly(true);
}

void cSpidercamDollyStatus::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QHBoxLayout(this);
	mainlayout->addWidget(mpX_Label);
	mainlayout->addWidget(mpX_m);
	mainlayout->addWidget(mpY_Label);
	mainlayout->addWidget(mpY_m);
	mainlayout->addWidget(mpZ_Label);
	mainlayout->addWidget(mpZ_m);
	mainlayout->addWidget(mpHeightLabel);
	mainlayout->addWidget(mpHeight_m);
	mainlayout->addWidget(mpPanLabel);
	mainlayout->addWidget(mpPan_deg);
	mainlayout->addWidget(mpTiltLabel);
	mainlayout->addWidget(mpTilt_deg);
	mainlayout->addWidget(mpSpeedLabel);
	mainlayout->addWidget(mpSpeed_mps);
	mainlayout->addWidget(mpBatteryLabel);
	mainlayout->addWidget(mpBatteryLevel_pct);

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
	
	setLayout(mainlayout);
}

void cSpidercamDollyStatus::setFloatingSize()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMinimumSize(300, 275);
	setMaximumSize(300, 275);
}

void cSpidercamDollyStatus::setHorzDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	setMinimumSize(1000, 50);
	setMaximumSize(16777215, 50);
}

void cSpidercamDollyStatus::setVertDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(300, 275);
	setMaximumSize(16777215, 16777215);
}

void cSpidercamDollyStatus::dockLocationChanged(Qt::DockWidgetArea area)
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

void cSpidercamDollyStatus::topLevelChanged(bool topLevel)
{
	// topLevel is true when our status view is floating. 
	if (topLevel)
	{
		verticalLayout();
		setFloatingSize();
	}
}


cBatteryStatus::cBatteryStatus(QWidget* parent)
	:
	QStatusBar(parent), mpBatteryLevel_pct(nullptr)
{
	setSizeGripEnabled(false);

	QFontMetrics fm(font());
	int pixelsWide = fm.horizontalAdvance("Bat: XXX ");

	mpBatteryLevel_pct = new QLineEdit(this);
	mpBatteryLevel_pct->setReadOnly(true);
	mpBatteryLevel_pct->setFixedWidth(pixelsWide);
	mpBatteryLevel_pct->setAlignment(Qt::AlignCenter);
	mpBatteryLevel_pct->setToolTip(tr("Battery Level (%)"));

	addPermanentWidget(mpBatteryLevel_pct);

	setWindowTitle("Battery Status");

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void cBatteryStatus::updateBatteryLevel(float level_pct)
{
	QString s = "Bat: ";
	s += QString::number(static_cast<uint8_t>(level_pct));

	mpBatteryLevel_pct->setText(s);
}
