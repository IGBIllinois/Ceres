

#include <QApplication>
#include <QStyleFactory>

#include "MainWindow.hpp"


int main(int argc, char** argv)
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setOrganizationName("University of Illinois");
    QCoreApplication::setOrganizationDomain("rapp.ripe.illinois.edu");
    QCoreApplication::setApplicationName("Ceres Experiment Planner");
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication app(argc, argv);

    cMainWindow mainWin;
    mainWin.initialize();
    mainWin.show();

	return app.exec();
}

