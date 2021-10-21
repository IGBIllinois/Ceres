
#pragma once

#include "GpsView.hpp"
//#include "GpsModel.hpp"
//#include "GpsStream_Qt.hpp"

#include <QWidget>



class GpsWidget : public cGpsView
{
public:
	GpsWidget(QWidget* parent = nullptr);
	virtual ~GpsWidget();

private:
//	GpsModel		mModel;
//	cGpsStream_Qt	mStream;
};