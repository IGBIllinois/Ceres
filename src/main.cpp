

#include <QApplication>
#include <QSplashScreen>

#include "MainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QPixmap* pPixmap = new QPixmap(":/ripe.illinois.edu/splash.png");
    QSplashScreen* pSplash = new QSplashScreen(*pPixmap);
    pSplash->show();

    cMainWindow mainWin;
    mainWin.initialize(pSplash);
    mainWin.show();

    pSplash->finish(&mainWin);

    delete pSplash; 
    pSplash = nullptr;
    
    delete pPixmap;
    pPixmap = nullptr;

	return app.exec();
}