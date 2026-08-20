
#pragma once

#include "../../Utilities/ColorGradient.hpp"

#include <QAbstractScrollArea>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cHyperspectralView : public QAbstractScrollArea
{
	Q_OBJECT

public:
	cHyperspectralView(QWidget* parent = nullptr);
	virtual ~cHyperspectralView();

public:
	/*
	 * This method is called by factory function to make sure the signals/slots
	 * between the model and status view are established
	 */
	virtual void connectToModel() = 0;

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	virtual void setFloatingSize();
	virtual void setDockedSize();

private:

	cColorGradient mColorGradient;
};