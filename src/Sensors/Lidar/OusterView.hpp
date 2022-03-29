
#pragma once

#include "../../Utilities/ColorGradient.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/OusterVizWidget.hpp>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cOusterModel;

class cOusterView : public cOusterVizWidget
{
	Q_OBJECT

public:
	cOusterView(cOusterModel* pModel, QWidget* parent = nullptr);
	virtual ~cOusterView();

public slots:
	void beamIntrinsicsChanged();
	void imuIntrinsicsChanged();
	void lidarIntrinsicsChanged();
	void dataFormatChanged();


	void azimuthWindowChanged(ouster::azimuth_range_t azimuth_range);
//	void encoderCountChanged(int min, int max);
	void imuDataChanged();

	void displayData();

public slots:
	void dockLocationChanged(Qt::DockWidgetArea area);
	void topLevelChanged(bool topLevel);

private:
	void setFloatingSize();
	void setDockedSize();

private:

/*
	double mGx;
	double mGy;
	double mGz;
*/

	cColorGradient mColorGradient;

protected:
	const cOusterModel* mpModel;
};