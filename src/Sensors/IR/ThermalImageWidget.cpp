
#include "ThermalImageWidget.hpp"

#include <QPaintEvent>
#include <QPainter>


cThermalImageWidget::cThermalImageWidget(QWidget* parent)
:
    QWidget(parent)
{
}

cThermalImageWidget::~cThermalImageWidget()
{
}

const QImage& cThermalImageWidget::getImage() const
{
    return mCurrentImage;
}

void cThermalImageWidget::setImage(const QImage& image)
{
    mCurrentImage = image;
    mAspectRatio = image.width() / static_cast<double>(image.height());
}

void cThermalImageWidget::resizeImage(int width, int height)
{
    mWindowWidth = width;
    mWindowHeight = height;
}

void cThermalImageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if ((mWindowWidth > 0) && (mWindowHeight > 0))
    {
        QRect rect(0, 0, mWindowWidth, mWindowHeight);

        int w = mWindowHeight * mAspectRatio;
        int h = mWindowWidth / mAspectRatio;

        if (w < mWindowWidth)
        {
            rect.setWidth(w);
        }
        else if (h < mWindowHeight)
        {
            rect.setHeight(h);
        }


        painter.drawImage(rect, mCurrentImage);
    }
    else
        painter.drawImage(0, 0, mCurrentImage);
}

