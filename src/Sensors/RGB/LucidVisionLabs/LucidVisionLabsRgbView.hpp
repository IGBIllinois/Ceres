
#pragma once

#include "../RgbCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class cLucidVisionLabsRgbModel;

class cLucidVisionLabsRgbView : public cRgbCameraView
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbView(cLucidVisionLabsRgbModel* pModel, QWidget* parent = nullptr);
	virtual ~cLucidVisionLabsRgbView();

public slots:
	virtual void imageUpdated(const QImage& image);

protected:
	const cLucidVisionLabsRgbModel* mpModel;
};