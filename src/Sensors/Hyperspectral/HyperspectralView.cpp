
#include "HyperspectralView.hpp"

#include <string>


cHyperspectralView::cHyperspectralView(QWidget* parent)
:
    QAbstractScrollArea(parent)
{
}

cHyperspectralView::~cHyperspectralView()
{
}

void cHyperspectralView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cHyperspectralView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cHyperspectralView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cHyperspectralView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

