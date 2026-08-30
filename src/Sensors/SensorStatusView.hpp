
#pragma once

#include <QWidget>

#include <memory>
#include <string>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QGroupBox;
QT_END_NAMESPACE

// Forward Declaration
class cSensorModel;
namespace sensor
{
	enum class eStatus;
}


class cSensorStatusView : public QWidget
{
	Q_OBJECT

public:
	cSensorStatusView(cSensorModel* pModel, QWidget* parent = nullptr);
	virtual ~cSensorStatusView();

public:
	/*
	 * Returns a string used as a descriptor of the sensor.
	 */
	virtual std::string descriptor() const;

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	virtual void createWidgets();
	virtual void doLayout() = 0;


public slots:
	virtual void onSensorStatusChange(QString name, QString instance, sensor::eStatus status);
	virtual void onSensorNameChanging(QString old_name, QString new_name, QString instance);

protected:
	QGroupBox* getSensorStatusBox() const;

private:
	cSensorModel* mpModel = nullptr;

private:
	QLabel*	   mpSensorLabel = nullptr;
	QLineEdit* mpSensorStatus = nullptr;
	QGroupBox* mpSensorStatusBox = nullptr;
};