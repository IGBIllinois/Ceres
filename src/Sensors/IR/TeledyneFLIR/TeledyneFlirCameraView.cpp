
#include "TeledyneFlirCameraView.hpp"
#include "Constants.hpp"
#include "TeledyneFlirCameraModel.hpp"

#include <QPaintEvent>
#include <QPainter>

#include <string>


cTeledyneFlirCameraView::cTeledyneFlirCameraView(cTeledyneFlirCameraModel* pModel, QWidget* parent)
	:
    cIrCameraView(parent),
    mpModel(pModel)
{
    setWindowTitle("IR Camera");
}

cTeledyneFlirCameraView::~cTeledyneFlirCameraView()
{
}

void cTeledyneFlirCameraView::imageUpdated(const QImage& image)
{
}

