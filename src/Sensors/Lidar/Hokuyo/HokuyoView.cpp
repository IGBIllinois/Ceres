
#include "HokuyoView.hpp"
#include "Constants.hpp"
#include "HokuyoModel.hpp"

//#include <ouster/ouster_utils.h>
#include <string>


namespace
{
//    const double ENCODER_TICS_TO_RAD = nConstants::TWO_PI / ouster::MAX_ENCODER_COUNT;

    float intensity2norm(uint16_t intensity)
    {
        const static float SCALE = 0.01f;
        const static float OFFSET = 1.0f;
        const static float MAX_VALUE = 1.225;

        if (intensity == 0) return 0.0f;

        float norm = log10f(intensity * SCALE + OFFSET) / MAX_VALUE;

        if (norm < 0.0f) return 0.0f;
        if (norm > 1.0f) return 1.0f;

        return norm;
    }
}

cHokuyoView::cHokuyoView(cHokuyoModel* pModel, QWidget* parent)
:
    QAbstractScrollArea(parent),
    mpModel(pModel)
{
    assert(pModel);
    setWindowTitle("Hokuyo LiDAR");
}

cHokuyoView::~cHokuyoView()
{
}


void cHokuyoView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cHokuyoView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cHokuyoView::dockLocationChanged(Qt::DockWidgetArea area)
{
    if ((area == Qt::LeftDockWidgetArea) || (area == Qt::RightDockWidgetArea))
    {
        setDockedSize();
        return;
    }

    if ((area == Qt::TopDockWidgetArea) || (area == Qt::BottomDockWidgetArea))
    {
        setDockedSize();
        return;
    }
}

void cHokuyoView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our GPS view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

void cHokuyoView::displayData()
{
    if (!isVisible()) return;
}


