
#include "SensorStatusView.hpp"
#include "SensorModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QFormLayout>

#include <string>

cSensorStatusView::cSensorStatusView(QWidget* parent)
:
	QWidget(parent)
{
}

cSensorStatusView::~cSensorStatusView()
{
}

void cSensorStatusView::createWidgets()
{
	mpSensorLabel = new QLabel();
	mpSensorLabel->setText("Sensor Status:");
	mpSensorStatus = new QLineEdit();
	mpSensorStatus->setReadOnly(true);
}

void cSensorStatusView::onSensorStatusChange(QString name, sensor::eStatus status)
{
	QString str = QString::fromStdString(sensor::to_string(status));
	mpSensorStatus->setText(str);
}
