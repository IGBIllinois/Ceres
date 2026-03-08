
#pragma once

#include "../IrCameraView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE

// Forward Declaration
class cTeledyneFlirCameraModel;

class cTeledyneFlirCameraView : public cIrCameraView
{
	Q_OBJECT

public:
	cTeledyneFlirCameraView(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
	virtual ~cTeledyneFlirCameraView();

//	void createWidgets() override;
//	void doLayout() override;

public slots:
	virtual void imageUpdated(const QImage& image);

protected:
	const cTeledyneFlirCameraModel* mpModel;
};