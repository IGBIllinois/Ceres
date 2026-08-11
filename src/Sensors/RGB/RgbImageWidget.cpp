
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

        int w_1 = static_cast<int>(mWindowWidth * 0.1);
        int h_1 = static_cast<int>(mWindowHeight * 0.1);

        int w_9 = static_cast<int>(mWindowWidth * 0.9);
        int h_9 = static_cast<int>(mWindowHeight * 0.9);

        painter.drawLine(w_1, h_1, w_1 + 50, h_1);
        painter.drawLine(w_1, h_1, w_1, h_1 + 50);

        painter.drawLine(w_9 - 50, h_1, w_9, h_1);
        painter.drawLine(w_9, h_1, w_9, h_1 + 50);

        painter.drawLine(w_1, h_9, w_1 + 50, h_9);
        painter.drawLine(w_1, h_9 - 50, w_1, h_9);

        painter.drawLine(w_9 - 50, h_9, w_9, h_9);
        painter.drawLine(w_9, h_9 - 50, w_9, h_9);

        painter.drawLine(w_1, h2 - 25, w_1, h2 + 25);
        painter.drawLine(w_9, h2 - 25, w_9, h2 + 25);

        painter.drawLine(w2 - 25, h_1, w2 + 25, h_1);
        painter.drawLine(w2 - 25, h_9, w2 + 25, h_9);

        int w_3 = static_cast<int>(mWindowWidth * 0.3);
        int h_3 = static_cast<int>(mWindowHeight * 0.3);

        int w_7 = static_cast<int>(mWindowWidth * 0.7);
        int h_7 = static_cast<int>(mWindowHeight * 0.7);

        painter.drawLine(w_3, h_3, w_3 + 50, h_3);
        painter.drawLine(w_3, h_3, w_3, h_3 + 50);

        painter.drawLine(w_7 - 50, h_3, w_7, h_3);
        painter.drawLine(w_7, h_3, w_7, h_3 + 50);

        painter.drawLine(w_3, h_7, w_3 + 50, h_7);
        painter.drawLine(w_3, h_7 - 50, w_3, h_7);

        painter.drawLine(w_7 - 50, h_7, w_7, h_7);
        painter.drawLine(w_7, h_7 - 50, w_7, h_7);

        painter.drawLine(w_3, h2 - 25, w_3, h2 + 25);
        painter.drawLine(w_7, h2 - 25, w_7, h2 + 25);

        painter.drawLine(w2 - 25, h_3, w2 + 25, h_3);
        painter.drawLine(w2 - 25, h_7, w2 + 25, h_7);
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
