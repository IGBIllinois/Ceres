
#pragma once

#include "../../Utilities/ColorGradient.hpp"

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cTofCameraView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cTofCameraView(QWidget* parent = nullptr);
	virtual ~cTofCameraView();

public slots:

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

private:

	cColorGradient mColorGradient;
};