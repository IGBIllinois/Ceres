
#include "TofCameraView.hpp"

#include <string>


cTofCameraView::cTofCameraView(QWidget* parent)
:
    QAbstractScrollArea(parent)
{
}

cTofCameraView::~cTofCameraView()
{
}

void cTofCameraView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cTofCameraView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cTofCameraView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cTofCameraView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

