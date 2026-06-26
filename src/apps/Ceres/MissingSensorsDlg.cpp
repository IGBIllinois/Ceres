
#include "MissingSensorsDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QStyle>
#include <QTextEdit>

#include <algorithm>


cMissingSensorsDlg::cMissingSensorsDlg(const QString& title, QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle(title);

	setMinimumWidth(500);

	createControls();
	createLayout();
}

cMissingSensorsDlg::~cMissingSensorsDlg()
{}

void cMissingSensorsDlg::addMissingSensor(const std::string& name)
{
	sMissingSensor_t info;
	info.name = name;

	mMissingSensors.push_back(info);
}

void cMissingSensorsDlg::addMissingSensor(const std::string& name, const std::string& manufacturer, const std::string& model, const std::string& sensor_name)
{
	sMissingSensor_t info;
	info.name = name;
	info.manufacturer = manufacturer;
	info.model = model;
	info.sensor_name = sensor_name;

	mMissingSensors.push_back(info);
}

void cMissingSensorsDlg::addSensor(const std::string& name, const std::string& manufacturer, const std::string& model, const std::string& sensor_name, const std::string& status)
{
	sSensor_t info;
	info.name = name;
	info.manufacturer = manufacturer;
	info.model = model;
	info.sensor_name = sensor_name;
	info.status = status;

	mSensors.push_back(info);
}

int cMissingSensorsDlg::exec()
{
	if (mMissingSensors.empty())
		mpMissingGroup->setHidden(true);
	else
	{
		QString table = "<table><tr><th width=100>Id</th><th width=100>Manufacturer</th><th width=100>Model</th><th width=100>Name</th></tr>";

		for (const auto& entry : mMissingSensors)
		{
			table += "<tr><td align=center>";
			table += entry.name;
			table += "</td><td align=center>";
			table += entry.manufacturer;
			table += "</td><td align=center>";
			table += entry.model;
			table += "</td><td align=center>";
			table += entry.sensor_name;
			table += "</td></tr>";
		}
		table += "</table>";
		mpMissingTable->setHtml(table);
	}

	if (mSensors.empty())
		mpSensorGroup->setHidden(true);
	else
	{
		QString table = "<table><tr><th width=100>Id</th><th width=100>Manufacturer</th><th width=100>Model</th><th width=100>Name</th><th width=100>Status</th></tr>";

		for (const auto& entry : mSensors)
		{
			table += "<tr><td align=center>";
			table += entry.name;
			table += "</td><td align=center>";
			table += entry.manufacturer;
			table += "</td><td align=center>";
			table += entry.model;
			table += "</td><td align=center>";
			table += entry.sensor_name;
			table += "</td><td align=center>";
			table += entry.status;
			table += "</td></tr>";
		}
		table += "</table>";
		mpSensorTable->setHtml(table);

		setMinimumWidth(600);
	}

	return QDialog::exec();
}

void cMissingSensorsDlg::createControls()
{
	QStyle* style = this->style();	// : QApplication::style();
	int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, nullptr, this);
	QIcon tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, nullptr, this);

	mpIconLabel = new QLabel(this);
	mpIconLabel->setPixmap(tmpIcon.pixmap(iconSize, iconSize));

	mpMissingGroup = new QGroupBox("Missing Sensors:", this);

	mpMissingTable = new QTextEdit(mpMissingGroup);
	mpMissingTable->setReadOnly(true);

	mpSensorGroup = new QGroupBox("Inactive Sensors:", this);
	mpSensorTable = new QTextEdit(mpSensorGroup);
	mpSensorTable->setReadOnly(true);
}

void cMissingSensorsDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pUpperLayout = new QHBoxLayout();

	pUpperLayout->addWidget(mpIconLabel);
	pUpperLayout->addStretch(1);
	pText = new QLabel("Not all of the required sensors are present or active!");
	pUpperLayout->addWidget(pText);
	pUpperLayout->addStretch(1);

	pMainLayout->addLayout(pUpperLayout);

	pMainLayout->addSpacing(10);

	QVBoxLayout* pMissingLayout = new QVBoxLayout();

	pMissingLayout->addWidget(mpMissingTable, 1);
	mpMissingGroup->setLayout(pMissingLayout);

	pMainLayout->addWidget(mpMissingGroup);

	pMainLayout->addSpacing(10);

	QVBoxLayout* pSensorLayout = new QVBoxLayout();

	pSensorLayout->addWidget(mpSensorTable, 1);

	mpSensorGroup->setLayout(pSensorLayout);

	pMainLayout->addWidget(mpSensorGroup);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	buttonBox->button(QDialogButtonBox::Ok)->setText("Run Anyways");
	buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancel Measurement");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

