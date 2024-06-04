
#include "ExperimentSensorInfoDlg.hpp"
#include "ExperimentSensorInfo.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QTabWidget>
#include <QTreeWidget>

#include <algorithm>


cExperimentSensorInfoDlg::cExperimentSensorInfoDlg(const std::vector<std::shared_ptr<cExperimentSensorInfo>>& info, QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Experiment Sensors");

	setMinimumWidth(350);

	mSensors = info;

	createControls();
	createLayout();
}

cExperimentSensorInfoDlg::~cExperimentSensorInfoDlg()
{}

const std::vector<std::shared_ptr<cExperimentSensorInfo>>& cExperimentSensorInfoDlg::getSensorInfo() const
{
	return mSensors;
}

void cExperimentSensorInfoDlg::createControls()
{
	mpAddSensor = new QPushButton(QIcon(":/ripe.illinois.edu/plus.png"), "", this);
	connect(mpAddSensor, &QPushButton::pressed, this, &cExperimentSensorInfoDlg::onAddSensor);

	mpRemoveSensor = new QPushButton(QIcon(":/ripe.illinois.edu/minus.png"), "", this);
	connect(mpRemoveSensor, &QPushButton::pressed, this, &cExperimentSensorInfoDlg::onRemoveSensor);

	mpSensorTabs = new QTabWidget(this);

	for (auto& sensor : mSensors)
	{
		mpSensorTabs->addTab(sensor->widget(), sensor->getName());
	}

	if (mSensors.size() == 1)
	{
		if (mSensors.front()->getType() == cExperimentSensorInfo_Dummy::type())
		{
			mpRemoveSensor->setDisabled(true);
		}
	}
}

void cExperimentSensorInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pLayout = new QHBoxLayout();

	pLayout->addStretch(1);
	pLayout->addWidget(mpAddSensor);
	pLayout->addWidget(mpRemoveSensor);

	pMainLayout->addLayout(pLayout);

	pMainLayout->addWidget(mpSensorTabs);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Reset);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cExperimentSensorInfoDlg::apply);
	connect(buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &cExperimentSensorInfoDlg::reset);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cExperimentSensorInfoDlg::accept()
{
	for (int i = 0; i < mpSensorTabs->count(); ++i)
	{
		auto* pWidget = static_cast<cSensorWidget*>(mpSensorTabs->widget(i));
		if (pWidget)
			pWidget->accept();
	}

	QDialog::accept();
}

void cExperimentSensorInfoDlg::apply()
{
	auto* pWidget = static_cast<cSensorWidget*>(mpSensorTabs->currentWidget());
	if (pWidget)
		pWidget->accept();
}

void cExperimentSensorInfoDlg::reset()
{
	auto* pWidget = static_cast<cSensorWidget*>(mpSensorTabs->currentWidget());
	if (pWidget)
		pWidget->reset();
}

void cExperimentSensorInfoDlg::onAddSensor()
{
	cSensorSelectDlg::sensor_info_t sensors;
	sensors.emplace_back(cSensorSelectDlg::sSensorInfo(cExperimentSensorInfo_Ouster::type(), "Ouster LiDAR" ));
	sensors.emplace_back(cSensorSelectDlg::sSensorInfo(cExperimentSensorInfo_Septentrio::type(), "Septentrio GPS"));
	sensors.emplace_back(cSensorSelectDlg::sSensorInfo(cExperimentSensorInfo_AxisCommunications::type(), "Axis Communications Webcam"));
	sensors.emplace_back(cSensorSelectDlg::sSensorInfo(cExperimentSensorInfo_VNIR3000N::type(), "HySpex VNIR-3000N"));
	sensors.emplace_back(cSensorSelectDlg::sSensorInfo(cExperimentSensorInfo_SWIR384::type(), "HySpex SWIR-384"));

	// Remove any sensor already in the list
	for (const auto& sensor : sensors)
	{
		for (auto it = mSensors.begin(); it != mSensors.end(); ++it)
		{
			if ((*it)->getType() == sensor.type)
			{
				mSensors.erase(it);
				break;
			}
		}
	}

	cSensorSelectDlg dlg("Add Sensors", this);
	dlg.initialize(sensors);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	auto sensors_to_add = dlg.selectedSensors();

	if (sensors_to_add.empty()) return;

	if (mSensors.size() == 1)
	{
		if (mSensors.front()->getType() == cExperimentSensorInfo_Dummy::type())
		{
			mpSensorTabs->removeTab(0);
		}
	}

	for (const auto& sensor : sensors_to_add)
	{
		auto pSensor = createSensor(sensor.type);
		mpSensorTabs->addTab(pSensor->widget(), pSensor->getName());
		mSensors.push_back(pSensor);
	}
}

void cExperimentSensorInfoDlg::onRemoveSensor()
{
	cSensorSelectDlg::sensor_info_t sensors; 
	for (const auto& sensor : mSensors)
	{
		sensors.emplace_back(cSensorSelectDlg::sSensorInfo(sensor->getType(), sensor->getName()));
	}

	cSensorSelectDlg dlg(" Remove Sensors ", this);
	dlg.initialize(sensors);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	auto sensors_to_remove = dlg.selectedSensors();

	// Remove the tab widgets
	for (const auto& sensor : sensors_to_remove)
	{
		for (int i = 0; i < mpSensorTabs->count(); ++i)
		{
			if (mpSensorTabs->tabText(i) == sensor.description)
			{
				mpSensorTabs->removeTab(i);
				break;
			}
		}
	}

	// Remove the sensor
	for (const auto& sensor_to_remove : sensors_to_remove)
	{
		for (auto it = mSensors.begin(); it != mSensors.end(); ++it)
		{
			if ((*it)->getType() == sensor_to_remove.type)
			{
				mSensors.erase(it);
				break;
			}
		}
	}

	if (mSensors.empty())
	{
		auto sensor = std::make_shared<cExperimentSensorInfo_Dummy>();
		mpSensorTabs->addTab(sensor->widget(), sensor->getName());

		mSensors.push_back(sensor);
	}

	if (mSensors.size() == 1)
	{
		if (mSensors.front()->getType() == cExperimentSensorInfo_Dummy::type())
		{
			mpRemoveSensor->setDisabled(true);
		}
	}
}


/*
 *  Selections Dialog
 */
cSensorSelectDlg::cSensorSelectDlg(const QString& title, QWidget* parent)
	:
	QDialog(parent)
{
	setWindowTitle(title);
}

cSensorSelectDlg::~cSensorSelectDlg()
{}

void cSensorSelectDlg::initialize(const sensor_info_t& sensors)
{
	mSensorInfo = sensors;

	mpSensors = new QTreeWidget(this);
	mpSensors->setColumnCount(1);
	mpSensors->setHeaderLabel("Possible Sensors");
	mpSensors->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpSensors->setSelectionBehavior(QAbstractItemView::SelectItems);
	mpSensors->setSelectionMode(QAbstractItemView::MultiSelection);

	auto n = sensors.size();

	QList<QTreeWidgetItem*> items;
	for (int i = 0; i < n; ++i)
	{
		auto* item = new QTreeWidgetItem(mpSensors);
		item->setText(0, sensors[i].description);
		items.append(item);
	}

	mpSensors->insertTopLevelItems(0, items);

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addWidget(mpSensors);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	buttonBox->button(QDialogButtonBox::Ok)->setText(windowTitle());

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

const cSensorSelectDlg::sensor_info_t& cSensorSelectDlg::selectedSensors()
{
	return mSensorInfo;
}

void cSensorSelectDlg::accept()
{
	sensor_info_t selected_sensor;
	auto items = mpSensors->selectedItems();
	for (const auto item : items)
	{
		for (const auto& sensor : mSensorInfo)
		{
			if (sensor.description == item->text(0))
			{
				selected_sensor.push_back(sensor);
			}
		}
	}

	mSensorInfo = selected_sensor;

	QDialog::accept();
}



