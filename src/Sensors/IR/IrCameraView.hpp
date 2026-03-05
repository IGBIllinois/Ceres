
#pragma once

#include "../../Utilities/ColorGradient.hpp"

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

public slots:

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);
	void resizeEvent(QResizeEvent*) override;

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

protected:
	cThermalImageWidget* mpImage = nullptr;

private:
	cColorGradient mColorGradient;
};