
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

public:
	/*
	 * This method is called by factory function to make sure the signals/slots
	 * between the model and status view are established
	 */
	virtual void connectToModel() = 0;

public slots:
	void updateCrossHairsColor(int r, int g, int b);
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