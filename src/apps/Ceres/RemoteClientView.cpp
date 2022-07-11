
#include "RemoteClientView.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QStackedLayout>

#include <string>


cRemoteClientView::cRemoteClientView(cCtrlDataModelRemote* pModel, QWidget* parent)
	:
	QAbstractScrollArea(parent),
	mpModel(pModel)
{
	createWidgets();
}

cRemoteClientView::~cRemoteClientView()
{

}

void cRemoteClientView::setConnectionStatus(const QString& status)
{
	mpConnectionStatus->setText(status);
	update();
}

void cRemoteClientView::enableReconnectButton(bool enable)
{
	if (enable)
		mpStatusReconnectLayout->setCurrentWidget(mpTryReconnect);
	else
		mpStatusReconnectLayout->setCurrentWidget(mpConnectionStatus);
	update();
}

void cRemoteClientView::tryReconnectPressed()
{
	mpModel->try_reconnection();
}

void cRemoteClientView::createWidgets()
{
	mpConnectionStatusLabel = new QLabel(this);
	mpConnectionStatusLabel->setText("Connection State:");
	mpConnectionStatus = new QLineEdit(this);
	mpConnectionStatus->setReadOnly(true);
	mpConnectionStatus->setAlignment(Qt::AlignCenter);

	mpTryReconnect = new QPushButton(this);
	mpTryReconnect->setText("Try to Reconnect");
//	mpTryReconnect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//	mpTryReconnect->setSizePolicy(QSizePolicy::controlType);
	connect(mpTryReconnect, &QPushButton::pressed, this, &cRemoteClientView::tryReconnectPressed);

	mpSensorLabel = new QLabel(this);
	mpSensorLabel->setText("OUSTER");
	mpSensorStatus = new QLineEdit(this);
	mpSensorStatus->setReadOnly(true);

	mpStatusReconnectLayout = new QStackedLayout;
	mpStatusReconnectLayout->addWidget(mpConnectionStatus);
	mpStatusReconnectLayout->addWidget(mpTryReconnect);
	mpStatusReconnectLayout->setCurrentWidget(mpConnectionStatus);

/*
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
*/
}

void cRemoteClientView::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QGridLayout(this);
	mainlayout->addWidget(mpConnectionStatusLabel, 0, 0);
//	mainlayout->addWidget(mpConnectionStatus, 0, 1);
	mainlayout->addLayout(mpStatusReconnectLayout, 0, 1);
	mainlayout->addWidget(mpSensorLabel, 0, 2);
	mainlayout->addWidget(mpSensorStatus, 0, 3);


/*
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
*/

	setLayout(mainlayout);
}

void cRemoteClientView::verticalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QFormLayout(this);
	mainlayout->addRow(mpConnectionStatusLabel, mpStatusReconnectLayout);
	mainlayout->addRow(mpSensorLabel, mpSensorStatus);

/*
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
*/

	setLayout(mainlayout);
}

void cRemoteClientView::setFloatingSize()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setMinimumSize(687, 139);
	setMaximumSize(687, 139);
}

void cRemoteClientView::setHorzDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	setMinimumSize(687, 139);
	setMaximumSize(16777215, 139);
}

void cRemoteClientView::setVertDockSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(250, 350);
	setMaximumSize(16777215, 16777215);
}

void cRemoteClientView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cRemoteClientView::topLevelChanged(bool topLevel)
{
	// topLevel is true when our view is floating. 
	if (topLevel)
	{
		horizontalLayout();
		setFloatingSize();
	}
}
