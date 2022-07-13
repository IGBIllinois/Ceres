
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
	setWindowTitle("Remote Status");
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
	mpTryReconnect->setEnabled(enable);

	update();
}

void cRemoteClientView::updateSensorStatus(const QString& sensor, const QString& status)
{
	for (auto sensor_status : mSensorStatus)
	{
		if (sensor_status.mpSensorLabel->text() == sensor)
		{
			sensor_status.mpSensorStatus->setText(status);
			update();
			return;
		}
	}

	sSensorStatus_t new_sensor;

	new_sensor.mpSensorLabel = new QLabel();
	new_sensor.mpSensorLabel->setText(sensor);
	new_sensor.mpSensorStatus = new QLineEdit();
	new_sensor.mpSensorStatus->setReadOnly(true);
	new_sensor.mpSensorStatus->setText(status);

	mSensorStatus.emplace_back(new_sensor);

	verticalLayout();

//	QFormLayout* formLayout = dynamic_cast<QFormLayout*>(layout());
//	if (formLayout)
//		formLayout->addRow(new_sensor.mpSensorLabel, new_sensor.mpSensorStatus);

	update();
}

void cRemoteClientView::sensorNameChange(const QString& old_name, const QString& new_name)
{
	for (auto sensor_status : mSensorStatus)
	{
		if (sensor_status.mpSensorLabel->text() == old_name)
		{
			sensor_status.mpSensorLabel->setText(new_name);
			update();
			return;
		}
	}
}

void cRemoteClientView::removeAllSensors()
{
	for (auto sensor : mSensorStatus)
	{
		sensor.mpSensorLabel->deleteLater();
		sensor.mpSensorStatus->deleteLater();
	}

	update();
	mSensorStatus.clear();
}

void cRemoteClientView::tryReconnectPressed()
{
	mpModel->try_reconnection();
}

void cRemoteClientView::createWidgets()
{
	mpConnectionStatusLabel = new QLabel();
	mpConnectionStatusLabel->setText("Connection State:");
	mpConnectionStatus = new QLineEdit();
	mpConnectionStatus->setReadOnly(true);
	mpConnectionStatus->setAlignment(Qt::AlignCenter);

	mpTryReconnect = new QPushButton();
	mpTryReconnect->setText("Try to Reconnect");
	mpTryReconnect->setEnabled(false);
//	mpTryReconnect->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	connect(mpTryReconnect, &QPushButton::pressed, this, &cRemoteClientView::tryReconnectPressed);

//	mpTryReconnect->setFixedHeight(mpConnectionStatus->sizeHint().height());
//	mpTryReconnect->setFixedSize(mpConnectionStatus->sizeHint());
//	mpConnectionStatus->setSizePolicy(mpTryReconnect->sizePolicy());
//	mpConnectionStatus->setFixedHeight(mpTryReconnect->sizeHint().height());

//	mpStatusReconnectLayout = new QStackedLayout();
//	mpStatusReconnectLayout->addWidget(mpConnectionStatus);
//	mpStatusReconnectLayout->addWidget(mpTryReconnect);
//	mpStatusReconnectLayout->setCurrentWidget(mpConnectionStatus);
//	mpStatusReconnectLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

void cRemoteClientView::horizontalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QGridLayout(this);
	mainlayout->addWidget(mpConnectionStatusLabel, 0, 0);
//	mainlayout->addLayout(mpStatusReconnectLayout, 0, 1);
	mainlayout->addWidget(mpConnectionStatus, 0, 1);

	setLayout(mainlayout);
}

void cRemoteClientView::verticalLayout()
{
	// We need to remove the old layout before we can add a new one!
	delete layout();

	auto* mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(mpTryReconnect, 1);

	auto* formlayout = new QFormLayout();
	formlayout->addRow(mpConnectionStatusLabel, mpConnectionStatus);

	for (auto sensor : mSensorStatus)
	{
		formlayout->addRow(sensor.mpSensorLabel, sensor.mpSensorStatus);
	}

	mainlayout->addLayout(formlayout);

	setLayout(mainlayout);
}

void cRemoteClientView::setFloatingSize()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
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
