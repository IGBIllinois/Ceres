
#pragma once

#include <string>
#include <nlohmann/json.hpp>


class cSensorModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
class QStatusBar;
//QT_END_NAMESPACE


struct sSensorWidgets
{
	cSensorModel* pModel;
	QDockWidget* pDockableView;
	QStatusBar* pStatusBar;

	// Future: property page

	sSensorWidgets() : pModel(nullptr), pDockableView(nullptr), pStatusBar(nullptr)
	{}

	sSensorWidgets(cSensorModel* model, QDockWidget* view, QStatusBar* status = nullptr)
		: pModel(model), pDockableView(view), pStatusBar(status)
	{}
};

sSensorWidgets create_sensor(const std::string& name, const nlohmann::json& sensorInfo);
void remove_sensor(const std::string& name, sSensorWidgets widgets);
