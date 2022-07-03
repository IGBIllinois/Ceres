

#include <QApplication>

#include "CeresSplashScreen.hpp"
#include "RemoteClientWindow.hpp"

int main(int argc, char** argv)
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);

    cCeresSplashScreen* pSplash = new cCeresSplashScreen();
    pSplash->show();

    cRemoteClientWindow mainWin;
    mainWin.initialize(pSplash);
    mainWin.show();

    pSplash->finish(&mainWin);

    delete pSplash; 
    pSplash = nullptr;

	return app.exec();
}

