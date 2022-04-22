
#include "RgbImageWidget.hpp"

#include <QPaintEvent>
#include <QPainter>


cRgbImageWidget::cRgbImageWidget(QWidget* parent)
:
    QWidget(parent)
{
}

cRgbImageWidget::~cRgbImageWidget()
{
}

const QImage& cRgbImageWidget::getImage() const
{
    return mCurrentImage;
}

void cRgbImageWidget::setImage(const QImage& image)
{
    mCurrentImage = image;
}

void cRgbImageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawImage(0, 0, mCurrentImage);
}

