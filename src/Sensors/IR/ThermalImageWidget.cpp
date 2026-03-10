
#include "ThermalImageWidget.hpp"

#include <QPaintEvent>
#include <QPainter>


cThermalImageWidget::cThermalImageWidget(QWidget* parent)
:
    QWidget(parent)
{
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::ColorRole::Base);
}

cThermalImageWidget::~cThermalImageWidget()
{
}

bool cThermalImageWidget::maintainingAspectRatio() const
{
    return mMaintainAspectRatio;
}

void cThermalImageWidget::maintainAspectRatio(bool enable)
{
    mMaintainAspectRatio = enable;
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

void cThermalImageWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    resizeImage(e->size().width(), e->size().height());
}

void cThermalImageWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    auto* engine = paintEngine();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if ((mWindowWidth > 0) && (mWindowHeight > 0))
    {
        QRect rect(0, 0, mWindowWidth, mWindowHeight);

        if (mMaintainAspectRatio)
        {
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
        }

        painter.drawImage(rect, mCurrentImage);
    }
    else
        painter.drawImage(0, 0, mCurrentImage);
}

