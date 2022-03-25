

#include <QApplication>

#include "CeresSplashScreen.hpp"
#include "MainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    cCeresSplashScreen* pSplash = new cCeresSplashScreen();
    pSplash->show();

    cMainWindow mainWin;
    mainWin.initialize(pSplash);
    mainWin.show();

    pSplash->finish(&mainWin);

    delete pSplash; 
    pSplash = nullptr;

	return app.exec();
}

