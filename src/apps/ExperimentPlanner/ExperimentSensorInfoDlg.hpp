
#pragma once

#include "ExperimentSensorInfo.hpp"

#include <QDialog>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QStackedLayout;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QTabWidget;
class QComboBox;
class QTreeWidget;
QT_END_NAMESPACE

#include <vector>
#include <memory>

class cExperimentSensorInfoDlg : public QDialog
{
	Q_OBJECT

public:
	cExperimentSensorInfoDlg(const std::vector<std::shared_ptr<cExperimentSensorInfo>>& info, QWidget* parent = nullptr);
	virtual ~cExperimentSensorInfoDlg();

	const std::vector<std::shared_ptr<cExperimentSensorInfo>>& getSensorInfo() const;

private slots:
	void accept() override;
	void apply();
	void reset();

	void onAddSensor();
	void onRemoveSensor();

private:
	void createControls();
	void createLayout();

private:
	std::vector<std::shared_ptr<cExperimentSensorInfo>> mSensors;

	QTabWidget* mpSensorTabs = nullptr;

	QPushButton* mpAddSensor = nullptr;
	QPushButton* mpRemoveSensor = nullptr;
};


class cSensorSelectDlg : public QDialog
{
	Q_OBJECT

public:
	struct sSensorInfo
	{
		std::string type;
		QString description;

		sSensorInfo(const char* t, const char* d) : type(t), description(d) {}
		sSensorInfo(const std::string& t, const QString& d) : type(t), description(d) {}
	};

	typedef std::vector<sSensorInfo> sensor_info_t;

public:
	cSensorSelectDlg(const QString& title, QWidget* parent = nullptr);
	~cSensorSelectDlg();

	void initialize(const sensor_info_t& sensors);

	const sensor_info_t& selectedSensors();

private slots:
	void accept() override;

private:
	QTreeWidget* mpSensors = nullptr;

	sensor_info_t mSensorInfo;
};

