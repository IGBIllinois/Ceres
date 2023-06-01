
#pragma once

#include <QAbstractScrollArea>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cHokuyoModel;

class cHokuyoView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cHokuyoView(cHokuyoModel* pModel, QWidget* parent = nullptr);
	virtual ~cHokuyoView();

public slots:
	void displayData();

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	void setFloatingSize();
	void setDockedSize();

private:

protected:
	const cHokuyoModel* mpModel;
};