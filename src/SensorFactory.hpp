
#pragma once

#include <string>

class cSensorModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
//QT_END_NAMESPACE


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

sSensorWidgets create_sensor(const std::string& name);
void remove_sensor(const std::string& name, sSensorWidgets widgets);
