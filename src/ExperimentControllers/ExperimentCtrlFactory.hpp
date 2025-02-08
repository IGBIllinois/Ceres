
#pragma once

#include <string>
#include <nlohmann/json.hpp>

// Forward Declaration
class cExperimentControlView;
class cExperimentControlModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
class QStatusBar;
class QToolBar;
//QT_END_NAMESPACE


struct sExperimentControllerWidgets
{
	cExperimentControlModel* pModel;
	cExperimentControlView* pView;
	QDockWidget* pDockableView;
	QStatusBar* pStatusBar;
	QToolBar* pToolBar;
	// Future: property page

	sExperimentControllerWidgets() : pModel(nullptr), pView(nullptr), pDockableView(nullptr),
		pStatusBar(nullptr), pToolBar(nullptr)
	{}

	sExperimentControllerWidgets(cExperimentControlModel* model, cExperimentControlView* view, 
		QDockWidget* dock = nullptr, QStatusBar* status = nullptr, QToolBar* toolbar = nullptr)
		: pModel(model), pView(view), pDockableView(dock), pStatusBar(status), pToolBar(toolbar)
	{}
};

sExperimentControllerWidgets create_experiment_controller(const std::string& name, const nlohmann::json& ctrlInfo, bool no_visualization = false);