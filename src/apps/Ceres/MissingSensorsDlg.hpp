
#pragma once

#include <QDialog>

#include <vector>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QGroupBox;
class QLabel;
class QTextEdit;
QT_END_NAMESPACE


class cMissingSensorsDlg : public QDialog
{
	Q_OBJECT

public:
	cMissingSensorsDlg(const QString& title, QWidget* parent = nullptr);
	virtual ~cMissingSensorsDlg();

	void addMissingSensor(const std::string& name);
	void addMissingSensor(const std::string& name, const std::string& manufacturer, const std::string& model, const std::string& sensor_name);

	void addSensor(const std::string& name, const std::string& manufacturer, const std::string& model, const std::string& sensor_name, const std::string& status);

	int exec() override;

private:
	void createControls();
	void createLayout();

private:

	struct sMissingSensor_t
	{
		std::string name;
		std::string manufacturer;
		std::string model;
		std::string sensor_name;
	};

	std::vector<sMissingSensor_t> mMissingSensors;

	struct sSensor_t
	{
		std::string name;
		std::string manufacturer;
		std::string model;
		std::string sensor_name;
		std::string status;
	};

	std::vector<sSensor_t> mSensors;

	QLabel*		mpIconLabel = nullptr;
	QGroupBox*	mpMissingGroup = nullptr;
	QTextEdit*	mpMissingTable = nullptr;
	QGroupBox*	mpSensorGroup = nullptr;
	QTextEdit*	mpSensorTable = nullptr;
};