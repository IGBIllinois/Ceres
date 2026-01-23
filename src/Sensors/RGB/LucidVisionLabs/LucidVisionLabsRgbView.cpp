
#include "LucidVisionLabsRgbView.hpp"
#include "Constants.hpp"
#include "LucidVisionLabsRgbModel.hpp"

#include <QPaintEvent>
#include <QPainter>

#include <string>


cLucidVisionLabsRgbView::cLucidVisionLabsRgbView(cLucidVisionLabsRgbModel* pModel, QWidget* parent)
	:
    cRgbCameraView(parent),
    mpModel(pModel)
{
    setWindowTitle("RGB Camera");
}

cLucidVisionLabsRgbView::~cLucidVisionLabsRgbView()
{
}

void cLucidVisionLabsRgbView::imageUpdated(const QImage& image)
{
    mpImage->setImage(image);
    if (!isHidden())
        mpImage->repaint();
}

