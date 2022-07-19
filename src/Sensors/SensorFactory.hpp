
#pragma once

#include <string>
#include <nlohmann/json.hpp>


class cSensorModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
class QStatusBar;
class QToolBar;
//QT_END_NAMESPACE


struct sSensorWidgets
{
	cSensorModel* pModel;
	QDockWidget* pDockableView;
	QStatusBar* pStatusBar;
	QToolBar* pToolBar;

	// Future: property page

	sSensorWidgets() : pModel(nullptr), pDockableView(nullptr), pStatusBar(nullptr),
		pToolBar(nullptr)
	{}

	sSensorWidgets(cSensorModel* model, QDockWidget* view, 
		QStatusBar* status = nullptr, QToolBar* toolbar = nullptr)
	:
		pModel(model), pDockableView(view), pStatusBar(status), pToolBar(toolbar)
	{}
};

sSensorWidgets create_sensor(const std::string& name, 
	const nlohmann::json& sensorInfo, bool no_visualization = false);

void remove_sensor(const std::string& name, sSensorWidgets widgets);
