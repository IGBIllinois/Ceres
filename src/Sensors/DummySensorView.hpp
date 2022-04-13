
#pragma once

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cDummyView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cDummyView(QWidget* parent = nullptr);
	virtual ~cDummyView();

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();
};