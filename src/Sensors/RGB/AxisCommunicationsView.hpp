
#pragma once

#include "RgbCameraView.hpp"


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QCamera;
QT_END_NAMESPACE

class cAxisCommunicationsModel;

class cAxisCommunicationsView : public cRgbCameraView
{
	Q_OBJECT

public:
	cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView();

public slots:


private:
	QCamera* mpCamera;

protected:
	const cAxisCommunicationsModel* mpModel;
};