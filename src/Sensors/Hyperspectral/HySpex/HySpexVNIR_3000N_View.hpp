
#pragma once

#include "../HyperspectralView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cHySpexVNIR_3000N_Model;

class cHySpexVNIR_3000N_View : public cHyperspectralView
{
	Q_OBJECT

public:
	cHySpexVNIR_3000N_View(cHySpexVNIR_3000N_Model* pModel, QWidget* parent = nullptr);
	virtual ~cHySpexVNIR_3000N_View();

	void connectToModel() override;

protected:
	const cHySpexVNIR_3000N_Model* mpModel;
};