
#pragma once

#include <string>
#include <QWidget>
#include <QDockWidget>

class cSensorModel;


struct sSensorWidgets
{
	cSensorModel* pModel;
	QDockWidget* pView;
	// Future: property page

	sSensorWidgets() : pModel(nullptr), pView(nullptr)
	{}

	sSensorWidgets(cSensorModel* model, QDockWidget* view)
		: pModel(model), pView(view)
	{}
};

sSensorWidgets create_sensor(const std::string& name, QWidget* parent);