
#include "MultispectralView.hpp"

#include <string>


cMultispectralView::cMultispectralView(QWidget* parent)
:
    QAbstractScrollArea(parent)
{
}

cMultispectralView::~cMultispectralView()
{
}

void cMultispectralView::setFloatingSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cMultispectralView::setDockedSize()
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(200, 200);
    setMaximumSize(16777215, 16777215);
}

void cMultispectralView::dockLocationChanged(Qt::DockWidgetArea area)
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

void cMultispectralView::topLevelChanged(bool topLevel)
{
    // topLevel is true when our view is floating. 
    if (topLevel)
    {
        setFloatingSize();
    }
}

