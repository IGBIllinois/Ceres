
#include "FieldLayoutWidget.hpp"

#include "Spidercam/SpidercamScanArea.hpp"


#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QPainter>
#include <QPainterPath>


cFieldLayoutWidget::cFieldLayoutWidget(QWidget* parent)
	: QWidget(parent)
{
}

void cFieldLayoutWidget::initialize()
{
    mpScanArea = new cSpidercamScanArea(this);

    auto* mainlayout = new QVBoxLayout();
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpScanArea);
    mainlayout->addSpacing(10);

    setLayout(mainlayout);

    mpScanArea->show();
}

/*
void cFieldLayoutWidget::paintEvent(QPaintEvent* event)
{
    mpScanArea->repaint();
    QWidget::paintEvent(event);
}
*/

