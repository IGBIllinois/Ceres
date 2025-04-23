
#pragma once

#include "../IrCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

class cTeledyneFlirCameraModel;

class cTeledyneFlirCameraView : public cIrCameraView
{
	Q_OBJECT

public:
	cTeledyneFlirCameraView(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cTeledyneFlirCameraView();

public slots:
	virtual void imageUpdated(const QImage& image);

protected:
	const cTeledyneFlirCameraModel* mpModel;
};