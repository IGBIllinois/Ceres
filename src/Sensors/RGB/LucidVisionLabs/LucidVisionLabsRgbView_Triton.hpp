
#pragma once

#include "LucidVisionLabsRgbView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
class QToolButton;
QT_END_NAMESPACE

class cLucidVisionLabsRgbModel_Triton;

class cLucidVisionLabsRgbView_Triton : public cLucidVisionLabsRgbView
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbView_Triton(cLucidVisionLabsRgbModel_Triton* pModel, QWidget* parent = nullptr);
	virtual ~cLucidVisionLabsRgbView_Triton();

	void initialize() override;

protected:
	const cLucidVisionLabsRgbModel_Triton* mpModel;
};