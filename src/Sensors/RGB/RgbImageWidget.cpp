
#include "RgbImageWidget.hpp"

#include <QPaintEvent>
#include <QPainter>


cRgbImageWidget::cRgbImageWidget(QWidget* parent)
:
    QWidget(parent)
{
    mColor.setRgb(255, 0, 255);
    mPen.setColor(mColor);
    mPen.setWidth(2);
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
    mAspectRatio = image.width() / static_cast<double>(image.height());
}

void cRgbImageWidget::showCrossHairs(bool enable)
{
    mShowCrossHairs = enable;
}

void cRgbImageWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    mWindowWidth = e->size().width();
    mWindowHeight = e->size().height();
}

void cRgbImageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if ((mWindowWidth > 0) && (mWindowHeight > 0))
    {
        mResizedImage = mCurrentImage.scaled(mWindowWidth, mWindowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter.drawImage(0, 0, mResizedImage);
    }
    else
    {
        painter.drawImage(0, 0, mCurrentImage);
    }

    if (mShowCrossHairs)
    {
        int w2 = mWindowWidth / 2;
        int h2 = mWindowHeight / 2;

        painter.setPen(mPen);

        painter.drawLine(0, h2, mWindowWidth, h2);
        painter.drawLine(w2, 0, w2, mWindowHeight);
    }
}


/*
void cRgbImageWidget::paintEvent(QPaintEvent* event)
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

        QImage resized = mCurrentImage.scaled(mWindowWidth, mWindowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter.drawImage(0, 0, resized);
        //        painter.drawImage(rect, mCurrentImage);

        auto image_size = mCurrentImage.size();
        painter.drawLine(0, 0, rect.width(), rect.height());
        painter.drawLine(0, 0, image_size.width(), image_size.height());
    }
    else
    {
        painter.drawImage(0, 0, mCurrentImage);

        auto image_size = mCurrentImage.size();
        painter.drawLine(0, 0, image_size.width(), image_size.height());
    }
}
*/
