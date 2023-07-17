
#pragma once

#include "../Sensors/SensorStatusView.hpp"
#include "QIndicator.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Forward Declaration
class cHySpexSWIR_384_Model;


class cHySpexSWIR_384_StatusView : public cSensorStatusView
{
	Q_OBJECT

public:
	cHySpexSWIR_384_StatusView(cHySpexSWIR_384_Model* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexSWIR_384_StatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;
	void doLayout() override;

public slots:


protected:
};