

#include "MainWindow.hpp"


#include <QApplication>
#include <QStyleFactory>
#include <QSurfaceFormat>

#include <SDL3/SDL_Init.h>

//#include <QVTKOpenGLNativeWidget.h>


int main(int argc, char* argv[])
{
//    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    SDL_Init(SDL_INIT_AUDIO);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setOrganizationName("University of Illinois");
    QCoreApplication::setOrganizationDomain("ceres.ripe.illinois.edu");
    QCoreApplication::setApplicationName("Ceres Field Mapper");
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication app(argc, argv);

    cMainWindow mainWin;
    mainWin.initialize();
    mainWin.show();

    auto result = app.exec();

    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    return result;
}
