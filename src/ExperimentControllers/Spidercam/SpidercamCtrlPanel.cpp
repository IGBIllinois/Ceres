
#include "SpidercamCtrlPanel.hpp"


#include <QFile>
#include <QDate>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QSettings>
#include <QHeaderView>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QStackedLayout>
#include <QDirIterator>
#include <QDebug>

#include <algorithm>
#include <fstream>
#include <string>


namespace
{
	const QString PLOT_SHIFT_TEXT = "Plot Shift (";
}


cSpidercamCtrlPanel::cSpidercamCtrlPanel(QWidget* parent) : QAbstractScrollArea(parent)
{
	setWindowTitle("Spidercam Controller");

	createWidgets();

	setFloatingSize();
	doLayout();
}

cSpidercamCtrlPanel::~cSpidercamCtrlPanel()
{}

void cSpidercamCtrlPanel::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;
}

void cSpidercamCtrlPanel::updatePosition(spidercam::sPosition_1_t pos)
{
	mX_mm = pos.X_mm;
	mY_mm = pos.Y_mm;
	mZ_mm = pos.Z_mm;
}

void cSpidercamCtrlPanel::onMoveEast()
{
	int step_size = mpHorizontalStepSize->text().toInt();
	emit stepY(step_size);
}

void cSpidercamCtrlPanel::onMoveWest()
{
	int step_size = -1 * mpHorizontalStepSize->text().toInt();
	emit stepY(step_size);
}

void cSpidercamCtrlPanel::onMoveSouth()
{
	int step_size = mpHorizontalStepSize->text().toInt();
	emit stepX(step_size);
}

void cSpidercamCtrlPanel::onMoveNorth()
{
	int step_size = -1 * mpHorizontalStepSize->text().toInt();
	emit stepX(step_size);
}

void cSpidercamCtrlPanel::onMoveEast10()
{
	int step_size = 10 * mpHorizontalStepSize->text().toInt();
	emit stepY(step_size);
}

void cSpidercamCtrlPanel::onMoveWest10()
{
	int step_size = -10 * mpHorizontalStepSize->text().toInt();
	emit stepY(step_size);
}

void cSpidercamCtrlPanel::onMoveSouth10()
{
	int step_size = 10 * mpHorizontalStepSize->text().toInt();
	emit stepX(step_size);
}

void cSpidercamCtrlPanel::onMoveNorth10()
{
	int step_size = -10 * mpHorizontalStepSize->text().toInt();
	emit stepX(step_size);
}

void cSpidercamCtrlPanel::onMoveUp()
{
	int step_size = mpVerticalStepSize->text().toInt();
	emit stepZ(step_size);
}

void cSpidercamCtrlPanel::onMoveDown()
{
	int step_size = -1 * mpVerticalStepSize->text().toInt();
	emit stepZ(step_size);
}

void cSpidercamCtrlPanel::onMoveUp10()
{
	int step_size = 10 * mpVerticalStepSize->text().toInt();
	emit stepZ(step_size);
}

void cSpidercamCtrlPanel::onMoveDown10()
{
	int step_size = -10 * mpVerticalStepSize->text().toInt();
	emit stepZ(step_size);
}


/****************************************************************************************
 * Helper Methods
 ****************************************************************************************/

void cSpidercamCtrlPanel::createWidgets()
{
	mpMoveEast = new QPushButton(this);
	mpMoveEast->setIcon(QIcon(":/spidercam.illinois.edu/images/up_arrow.png"));
	connect(mpMoveEast, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveEast);

	mpMoveWest = new QPushButton(this);
	mpMoveWest->setIcon(QIcon(":/spidercam.illinois.edu/images/down_arrow.png"));
	connect(mpMoveWest, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveWest);

	mpMoveSouth = new QPushButton(this);
	mpMoveSouth->setIcon(QIcon(":/spidercam.illinois.edu/images/right_arrow.png"));
	connect(mpMoveSouth, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveSouth);

	mpMoveNorth = new QPushButton(this);
	mpMoveNorth->setIcon(QIcon(":/spidercam.illinois.edu/images/left_arrow.png"));
	connect(mpMoveNorth, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveNorth);

	mpMoveEast10 = new QPushButton(this);
	mpMoveEast10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_up_arrow.png"));
	connect(mpMoveEast10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveEast10);

	mpMoveWest10 = new QPushButton(this);
	mpMoveWest10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_down_arrow.png"));
	connect(mpMoveWest10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveWest10);

	mpMoveSouth10 = new QPushButton(this);
	mpMoveSouth10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_right_arrow.png"));
	connect(mpMoveSouth10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveSouth10);

	mpMoveNorth10 = new QPushButton(this);
	mpMoveNorth10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_left_arrow.png"));
	connect(mpMoveNorth10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveNorth10);

	mpHorizontalStepSize = new QLineEdit(this);
	mpHorizontalStepSize->setValidator(new QDoubleValidator(0, 10'000.0, 3));
	mpHorizontalStepSize->setText(QString::number(500));

	mpMoveUp = new QPushButton(this);
	mpMoveUp->setIcon(QIcon(":/spidercam.illinois.edu/images/up_arrow.png"));
	connect(mpMoveUp, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveUp);

	mpMoveDown = new QPushButton(this);
	mpMoveDown->setIcon(QIcon(":/spidercam.illinois.edu/images/down_arrow.png"));
	connect(mpMoveDown, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveDown);

	mpMoveUp10 = new QPushButton(this);
	mpMoveUp10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_up_arrow.png"));
	connect(mpMoveUp10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveUp10);

	mpMoveDown10 = new QPushButton(this);
	mpMoveDown10->setIcon(QIcon(":/spidercam.illinois.edu/images/double_down_arrow.png"));
	connect(mpMoveDown10, &QPushButton::pressed, this, &cSpidercamCtrlPanel::onMoveDown10);

	mpVerticalStepSize = new QLineEdit(this);
	mpVerticalStepSize->setValidator(new QDoubleValidator(0, 1'000.0, 3));
	mpVerticalStepSize->setText(QString::number(50));
}

//-----------------------------------------------------------------------------

#if 0
void cSpidercamCtrlPanel::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QHBoxLayout(this);

	QGridLayout* pHeightLayout = new QGridLayout();
	pHeightLayout->setColumnMinimumWidth(2, 10);

	pHeightLayout->addWidget(mpMoveUp10, 0, 3);
	pHeightLayout->addWidget(mpMoveUp, 1, 3);
	pHeightLayout->addWidget(mpMoveDown, 3, 3);
	pHeightLayout->addWidget(mpMoveDown10, 4, 3);

	mainlayout->addLayout(pHeightLayout);

	mainlayout->addSpacing(20);

	QGridLayout* pXY_Layout = new QGridLayout();
	pXY_Layout->setColumnMinimumWidth(2, 10);

	pXY_Layout->addWidget(mpMoveEast10, 0, 3);
	pXY_Layout->addWidget(mpMoveEast, 1, 3);
	pXY_Layout->addWidget(mpMoveNorth10, 3, 0);
	pXY_Layout->addWidget(mpMoveNorth, 3, 1);
	pXY_Layout->addWidget(mpMoveSouth, 3, 5);
	pXY_Layout->addWidget(mpMoveSouth10, 3, 6);
	pXY_Layout->addWidget(mpMoveWest, 5, 3);
	pXY_Layout->addWidget(mpMoveWest10, 6, 3);

	mainlayout->addLayout(pXY_Layout);
	mainlayout->addSpacing(10);

	setLayout(mainlayout);
}
#endif

void cSpidercamCtrlPanel::doLayout()
{
	// We need to remove the old layout before we can add a new one!
//	delete layout();

	QVBoxLayout* mainlayout = new QVBoxLayout(this);

	QGroupBox* vertBox  = new QGroupBox();

	vertBox->setTitle("Veritcal Movement");

	QVBoxLayout* vertlayout = new QVBoxLayout(this);

	QGridLayout* pHeightLayout = new QGridLayout();
	pHeightLayout->setColumnMinimumWidth(2, 10);

	pHeightLayout->addWidget(new QLabel("Up"), 0, 3, Qt::AlignCenter);
	pHeightLayout->addWidget(mpMoveUp10, 1, 3);
	pHeightLayout->addWidget(mpMoveUp, 2, 3);
	pHeightLayout->addWidget(mpMoveDown, 4, 3);
	pHeightLayout->addWidget(mpMoveDown10, 5, 3);
	pHeightLayout->addWidget(new QLabel("Down"), 6, 3, Qt::AlignCenter);

	vertlayout->addLayout(pHeightLayout);

	vertlayout->addSpacing(10);

	QHBoxLayout* vertSizelayout = new QHBoxLayout(this);

	vertSizelayout->addStretch(1);

	QLabel* pVertLabel = new QLabel("Height Step Size (mm): ");
	vertSizelayout->addWidget(pVertLabel);
	vertSizelayout->addWidget(mpVerticalStepSize);

	vertSizelayout->addStretch(1);

	vertlayout->addLayout(vertSizelayout);

	vertBox->setLayout(vertlayout);

	mainlayout->addWidget(vertBox, 0);

	mainlayout->addSpacing(20);


	QGroupBox* xyBox = new QGroupBox();

	xyBox->setTitle("East/West South/North Movement");

	QVBoxLayout* xySublayout = new QVBoxLayout();

	QGridLayout* pXY_Layout = new QGridLayout();
	pXY_Layout->setColumnMinimumWidth(2, 10);

	pXY_Layout->addWidget(new QLabel("East"), 0, 3, Qt::AlignCenter);
	pXY_Layout->addWidget(mpMoveEast10, 1, 3);
	pXY_Layout->addWidget(mpMoveEast, 2, 3);
	pXY_Layout->addWidget(new QLabel("North"), 4, 0, Qt::AlignCenter);
	pXY_Layout->addWidget(mpMoveNorth10, 4, 1);
	pXY_Layout->addWidget(mpMoveNorth, 4, 2);
	pXY_Layout->addWidget(mpMoveSouth, 4, 6);
	pXY_Layout->addWidget(mpMoveSouth10, 4, 7);
	pXY_Layout->addWidget(new QLabel("South"), 4, 8, Qt::AlignCenter);
	pXY_Layout->addWidget(mpMoveWest, 6, 3);
	pXY_Layout->addWidget(mpMoveWest10, 7, 3);
	pXY_Layout->addWidget(new QLabel("West"), 8, 3, Qt::AlignCenter);

	xySublayout->addLayout(pXY_Layout);

	xySublayout->addSpacing(10);

	QHBoxLayout* xySizelayout = new QHBoxLayout();

	xySizelayout->addStretch(1);

	QLabel* pXyLabel = new QLabel("Step Size (mm): ");
	xySizelayout->addWidget(pXyLabel);
	xySizelayout->addWidget(mpHorizontalStepSize);

	xySizelayout->addStretch(1);

	xySublayout->addLayout(xySizelayout);

	xyBox->setLayout(xySublayout);

	mainlayout->addWidget(xyBox);

	setLayout(mainlayout);
}

void cSpidercamCtrlPanel::setFloatingSize()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMinimumSize(300, 500);
	setMaximumSize(300, 500);
}

#if 0
void cSpidercamCtrlPanel::setHorzDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	setMinimumSize(1000, 50);
	setMaximumSize(16777215, 50);
}

void cSpidercamCtrlPanel::setVertDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(300, 275);
	setMaximumSize(16777215, 16777215);
}

void cSpidercamCtrlPanel::dockLocationChanged(Qt::DockWidgetArea area)
{
/*
	if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea))
	{
		verticalLayout();
		setVertDockSize();
		return;
	}

	if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea))
	{
//		horizontalLayout();
//		setHorzDockSize();
		verticalLayout();
		setVertDockSize();
		return;
	}
*/
}

void cSpidercamCtrlPanel::topLevelChanged(bool topLevel)
{
/*
	// topLevel is true when our status view is floating. 
	if (topLevel)
	{
		verticalLayout();
		setFloatingSize();
	}
*/
}
#endif

