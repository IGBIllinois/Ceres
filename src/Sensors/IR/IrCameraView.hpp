
#pragma once

//#include "../../Utilities/ColorGradient.hpp"

#include "ThermalColorTypes.hpp"

#include "ThermalImageWidget.hpp"

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cIrCameraView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cIrCameraView(QWidget* parent = nullptr);
	virtual ~cIrCameraView();

public:
	/*
	 * This method is called by factory function to make sure the signals/slots
	 * between the model and status view are established
	 */
	virtual void connectToModel() = 0;

signals:
	void requestColorPalette(eColorTable palette);

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

protected:
	cThermalImageWidget* mpThermalImage = nullptr;

private:
//	cColorGradient mColorGradient;
};