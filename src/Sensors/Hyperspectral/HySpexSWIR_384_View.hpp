
#pragma once

#include "HyperspectralView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class cHySpexSWIR_384_Model;

class cHySpexSWIR_384_View : public cHyperspectralView
{
	Q_OBJECT

public:
	cHySpexSWIR_384_View(QWidget* parent = nullptr);
	virtual ~cHySpexSWIR_384_View();

public slots:


protected:
	const cHySpexSWIR_384_Model* mpModel;
};