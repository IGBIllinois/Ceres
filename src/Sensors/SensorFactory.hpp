
#pragma once

#include <string>
#include <nlohmann/json.hpp>


class cSensorModel;
class cSensorPropertyPage;
class cSensorPropertyPageRemote;
class cSensorController;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
class QStatusBar;
class QToolBar;
//QT_END_NAMESPACE


struct sSensorWidgets
{
	cSensorModel*			pModel = nullptr;
	QDockWidget*			pDockableView = nullptr;
	QStatusBar*				pStatusBar = nullptr;
	QToolBar*				pToolBar = nullptr;
	cSensorPropertyPage*	pPropertyPage = nullptr;
	cSensorController*		pController = nullptr;

	sSensorWidgets() = default;

	sSensorWidgets(cSensorModel* model, QDockWidget* view, 
		QStatusBar* status = nullptr, QToolBar* toolbar = nullptr,
		cSensorPropertyPage* properties = nullptr, 
		cSensorController* controller = nullptr)
	:
		pModel(model), pDockableView(view), pStatusBar(status), pToolBar(toolbar),
		pPropertyPage(properties), pController(controller)
	{}
};


sSensorWidgets create_sensor(const std::string& name, 
	const nlohmann::json& sensorInfo, bool no_visualization = false);

void remove_sensor(const std::string& name, sSensorWidgets widgets);

cSensorPropertyPageRemote* create_sensor_property_page(const std::string& name, uint32_t version);

