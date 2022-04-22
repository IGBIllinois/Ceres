
#include "AxisCommunicationsView.hpp"
#include "../../Utilities/Constants.hpp"
#include "AxisCommunicationsModel.hpp"

#include <QPaintEvent>
#include <QPainter>

#include <string>


cAxisCommunicationsView::cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent)
	:
    cRgbCameraView(),
    mpModel(pModel)
{
    setWindowTitle("RGB Camera");
}

cAxisCommunicationsView::~cAxisCommunicationsView()
{
}

void cAxisCommunicationsView::imageUpdated(const QImage& image)
{
    mpImage->setImage(image);
    if (!isHidden())
        mpImage->repaint();
}

