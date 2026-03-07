
#include "TeledyneFlirCameraView_T1K.hpp"
#include "Constants.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"

#include <QPaintEvent>
#include <QPainter>

#include <string>


cTeledyneFlirCameraView_T1K::cTeledyneFlirCameraView_T1K(cTeledyneFlirCameraModel_T1K* pModel, QWidget* parent)
	:
    cTeledyneFlirCameraView(pModel, parent)
{
}

cTeledyneFlirCameraView_T1K::~cTeledyneFlirCameraView_T1K()
{
}

void cTeledyneFlirCameraView_T1K::initialize()
{

}
