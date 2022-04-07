
#pragma once

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

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

private:
};