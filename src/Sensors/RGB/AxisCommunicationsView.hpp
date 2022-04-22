
#pragma once

#include "RgbCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class cAxisCommunicationsModel;

class cAxisCommunicationsView : public cRgbCameraView
{
	Q_OBJECT

public:
	cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
	virtual ~cAxisCommunicationsView();

public slots:
	void imageUpdated(const QImage& image);

protected:
	const cAxisCommunicationsModel* mpModel;
};