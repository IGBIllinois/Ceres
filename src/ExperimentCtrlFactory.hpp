
#pragma once

#include <string>

class cExperimentControlView;
class cExperimentControlModel;

// Qt Forward Declaration
//QT_BEGIN_NAMESPACE
class QDockWidget;
//QT_END_NAMESPACE


struct sExperimentControllerWidgets
{
	cExperimentControlModel* pModel;
	cExperimentControlView* pView;
	QDockWidget* pDockableView;
	// Future: property page

	sExperimentControllerWidgets() : pModel(nullptr), pView(nullptr), pDockableView(nullptr)
	{}

	sExperimentControllerWidgets(cExperimentControlModel* model, cExperimentControlView* view)
		: pModel(model), pView(view), pDockableView(nullptr)
	{}

	sExperimentControllerWidgets(cExperimentControlModel* model, cExperimentControlView* view, QDockWidget* dock)
		: pModel(model), pView(view), pDockableView(dock)
	{}
};

sExperimentControllerWidgets create_experiment_controller(const std::string& name);