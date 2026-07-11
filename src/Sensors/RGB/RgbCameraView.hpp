
#pragma once

#include "RgbImageWidget.hpp"

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cRgbCameraView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cRgbCameraView(QWidget* parent = nullptr);
	virtual ~cRgbCameraView();

public slots:
	void enableCrosshairs(bool enable);

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);
	void resizeEvent(QResizeEvent*) override;

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

protected:
	cRgbImageWidget* mpImage = nullptr;
};