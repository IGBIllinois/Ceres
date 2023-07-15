
#pragma once

#include <string>
#include <nlohmann/json.hpp>


class cSensorModel;
class cSensorPropertyPage;
class cSensorController;
class cSensorStatusView;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QWidget;
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
	cSensorStatusView*		pRemoteStatusView = nullptr;

	sSensorWidgets() = default;

	sSensorWidgets(cSensorModel* model)
		:
		pModel(model), pDockableView(nullptr), pStatusBar(nullptr), pToolBar(nullptr),
		pPropertyPage(nullptr), pController(nullptr), pRemoteStatusView(nullptr)
	{}

	sSensorWidgets(cSensorModel* model, QDockWidget* view, cSensorPropertyPage* properties)
		:
		pModel(model), pDockableView(view), pStatusBar(nullptr), pToolBar(nullptr),
		pPropertyPage(properties), pController(nullptr), pRemoteStatusView(nullptr)
	{}

	sSensorWidgets(cSensorModel* model, QDockWidget* view, 
		QStatusBar* status = nullptr, QToolBar* toolbar = nullptr,
		cSensorPropertyPage* properties = nullptr, 
		cSensorController* controller = nullptr, 
		cSensorStatusView* remoteStatusView = nullptr)
	:
		pModel(model), pDockableView(view), pStatusBar(status), pToolBar(toolbar),
		pPropertyPage(properties), pController(controller), pRemoteStatusView(remoteStatusView)
	{}

	sSensorWidgets(cSensorModel* model, cSensorController* controller, cSensorStatusView* remoteStatusView = nullptr)
		:
		pModel(model), pDockableView(nullptr), pStatusBar(nullptr), pToolBar(nullptr),
		pPropertyPage(nullptr), pController(controller), pRemoteStatusView(remoteStatusView)
	{}

	operator bool() const { return static_cast<bool>(pModel); }
};


sSensorWidgets create_sensor(const std::string& sensor_id,
	const nlohmann::json& sensorInfo, bool no_visualization = false);

void remove_sensor(const std::string& sensor_id, sSensorWidgets widgets);

cSensorPropertyPage* create_sensor_property_page(const std::string& sensor_id,
	const std::string& model, uint32_t version,
	const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);

