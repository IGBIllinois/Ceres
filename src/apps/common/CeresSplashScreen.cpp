
#include "CeresSplashScreen.hpp"

#include <QPainter>

cCeresSplashScreen::cCeresSplashScreen()
:
    QSplashScreen(), mpPixmap(nullptr)
{
    mpPixmap = new QPixmap(":/ripe.illinois.edu/splash.png");
    setPixmap(*mpPixmap);
}

cCeresSplashScreen::~cCeresSplashScreen()
{
    delete mpPixmap;
    mpPixmap = nullptr;
}


void cCeresSplashScreen::showMessage(const QString& message)
{
    mMessage = message;
    repaint();
//    QSplashScreen::showMessage(message, Qt::AlignHCenter | Qt::AlignBottom, Qt::black);
}

/*!
    Draw the contents of the splash screen using painter \a painter.
    The default implementation draws the message passed by showMessage().
    Reimplement this function if you want to do your own drawing on
    the splash screen.
*/
void cCeresSplashScreen::drawContents(QPainter* painter)
{
    painter->setPen(Qt::black);
    QRect r = rect().adjusted(10, 10, -10, -10);
    painter->drawText(r, Qt::AlignHCenter | Qt::AlignBottom, mMessage);
}
