

#include <QApplication>

#include "CeresSplashScreen.hpp"
#include "MainWindow.hpp"


#include <fstream>

static std::ofstream g_logFile;


void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* file = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        g_logFile << "Debug: " << localMsg.constData() << " (" << file;
        g_logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtInfoMsg:
        g_logFile << "Info: " << localMsg.constData() << " (" << file;
        g_logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtWarningMsg:
        g_logFile << "Warning: " << localMsg.constData() << " (" << file;
        g_logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtCriticalMsg:
        g_logFile << "Critical: " << localMsg.constData() << " (" << file;
        g_logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtFatalMsg:
        g_logFile << "Fatal: " << localMsg.constData() << " (" << file;
        g_logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    }
}


int main(int argc, char** argv)
{
    g_logFile.open("ceres.log", std::ios::trunc);

    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setOrganizationName("University of Illinois");
    QCoreApplication::setOrganizationDomain("rapp.ripe.illinois.edu");
    QCoreApplication::setApplicationName("Ceres");

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

