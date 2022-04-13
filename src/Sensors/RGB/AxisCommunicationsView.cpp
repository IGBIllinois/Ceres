
#include "AxisCommunicationsView.hpp"
#include "../../Utilities/Constants.hpp"
#include "AxisCommunicationsModel.hpp"

#include <QCamera>

#include <string>


cAxisCommunicationsView::cAxisCommunicationsView(cAxisCommunicationsModel* pModel, QWidget* parent)
	:
    cRgbCameraView(),
    mpCamera(nullptr),
    mpModel(pModel)
{
    setWindowTitle("RGB Camera");
}

cAxisCommunicationsView::~cAxisCommunicationsView()
{
}


