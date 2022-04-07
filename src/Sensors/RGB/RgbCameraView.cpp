
#include "RgbCameraView.hpp"

#include <string>


cRgbCameraView::cRgbCameraView(QWidget* parent)
:
    QAbstractScrollArea(parent)
{
}

cRgbCameraView::~cRgbCameraView()
{
}

void cRgbCameraView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cRgbCameraView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cRgbCameraView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cRgbCameraView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

