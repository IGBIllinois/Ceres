
#pragma once

#include <QWidget>

#include <memory>

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
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	virtual void createWidgets();
	virtual void doLayout() = 0;

public slots:
	virtual void onSensorStatusChange(QString name, QString instance, sensor::eStatus status);

protected:
	QGroupBox* getSensorStatusBox() const;

private:
	QLabel*	   mpSensorLabel = nullptr;
	QLineEdit* mpSensorStatus = nullptr;
	QGroupBox* mpSensorStatusBox = nullptr;
};