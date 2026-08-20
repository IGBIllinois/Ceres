
#pragma once

#include "HySpexCamera_StatusView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

// Forward Declaration
class cHySpexVNIR_3000N_Model;
class QLedIndicator;


class cHySpexVNIR_3000N_StatusView : public cHySpexCamera_StatusView
{
	Q_OBJECT

public:
	cHySpexVNIR_3000N_StatusView(cHySpexVNIR_3000N_Model* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexVNIR_3000N_StatusView();

public:
	/*
	 * These methods are called by factory function to make sure the GUI elements are
	 * created and displayed in the correct arrangment
	 */
	void createWidgets() override;
	void doLayout() override;

	void connectToModel() override;

private:
	cHySpexVNIR_3000N_Model* mpModel = nullptr;
};