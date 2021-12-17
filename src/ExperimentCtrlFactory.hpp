
#pragma once

#include <string>

class cExperimentControlView;
class cExperimentControlModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
class QStatusBar;
//QT_END_NAMESPACE


struct sExperimentControllerWidgets
{
	cExperimentControlModel* pModel;
	cExperimentControlView* pView;
	QDockWidget* pDockableView;
	QStatusBar* pStatusBar;
	// Future: property page

	sExperimentControllerWidgets() : pModel(nullptr), pView(nullptr), pDockableView(nullptr), pStatusBar(nullptr)
	{}

	sExperimentControllerWidgets(cExperimentControlModel* model, cExperimentControlView* view, 
		QDockWidget* dock = nullptr, QStatusBar* status = nullptr)
		: pModel(model), pView(view), pDockableView(dock), pStatusBar(status)
	{}
};

sExperimentControllerWidgets create_experiment_controller(const std::string& name);