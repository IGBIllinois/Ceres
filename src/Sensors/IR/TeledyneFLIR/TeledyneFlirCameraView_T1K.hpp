
#pragma once

#include "TeledyneFlirCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class cTeledyneFlirCameraModel_T1K;

class cTeledyneFlirCameraView_T1K : public cTeledyneFlirCameraView
{
	Q_OBJECT

public:
	cTeledyneFlirCameraView_T1K(cTeledyneFlirCameraModel_T1K* pModel, QWidget* parent = nullptr);
	virtual ~cTeledyneFlirCameraView_T1K();

	void initialize() override;

private:
	cTeledyneFlirCameraModel_T1K* mpModel = nullptr;
};