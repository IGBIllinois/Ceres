
#pragma once

#include "HySpexCamera_StatusView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Forward Declaration
class cHySpexSWIR_384_Model;


class cHySpexSWIR_384_StatusView : public cHySpexCamera_StatusView
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

	void onSensorTempChange(double temp_C) override;
};