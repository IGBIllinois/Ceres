
#include "SensorStatusView.hpp"
#include "SensorModel.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>

#include <string>

cSensorStatusView::cSensorStatusView(cSensorModel* pModel, QWidget* parent)
:
	QWidget(parent)
{
	setWindowTitle(pModel->name().c_str());
}

cSensorStatusView::~cSensorStatusView()
{
}

void cSensorStatusView::createWidgets()
{
	mpSensorLabel = new QLabel();
	mpSensorLabel->setText("Status:");
	mpSensorStatus = new QLineEdit();
	mpSensorStatus->setReadOnly(true);
	mpSensorStatus->setText("UNKNOWN");

	mpSensorStatusBox = new QGroupBox("Sensor Status");

	auto* sensorInfoLayout = new QFormLayout();
	sensorInfoLayout->addRow(mpSensorLabel, mpSensorStatus);

	mpSensorStatusBox->setLayout(sensorInfoLayout);
}

void cSensorStatusView::onSensorStatusChange(QString name, sensor::eStatus status)
{
	QString str = QString::fromStdString(sensor::to_string(status));
	mpSensorStatus->setText(str);
}

QGroupBox* cSensorStatusView::getSensorStatusBox() const
{
	return mpSensorStatusBox;
}

