
#include "IrCameraView.hpp"

#include <QResizeEvent>
#include <string>


cIrCameraView::cIrCameraView(QWidget* parent)
:
    QAbstractScrollArea(parent)
{
    mpThermalImage = new cThermalImageWidget(this);
}

cIrCameraView::~cIrCameraView()
{
}

void cIrCameraView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cIrCameraView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cIrCameraView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cIrCameraView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}


