

#include <QApplication>
#include <QStyleFactory>

#include "CeresSplashScreen.hpp"
#include "MainWindow.hpp"

#include <QDateTime>

#include <SDL3/SDL_Init.h>

#include <fstream>

static std::ofstream g_logFile;


void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    bool hasLocation = (context.file ? true : false) || (context.function ? true : false);
    const char* file = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";

    g_logFile << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ").toStdString();

    switch (type) {
    case QtDebugMsg:
        g_logFile << "Debug: " << localMsg.constData();
        break;
    case QtInfoMsg:
        g_logFile << "Info: " << localMsg.constData();
        break;
    case QtWarningMsg:
        g_logFile << "Warning: " << localMsg.constData();
        break;
    case QtCriticalMsg:
        g_logFile << "Critical: " << localMsg.constData();
        break;
    case QtFatalMsg:
        g_logFile << "Fatal: " << localMsg.constData();
        break;
    }

    if (hasLocation)
    {
        g_logFile << " (" << file << ":" << context.line << ", " << function << ")";
    }
    g_logFile << std::endl;
}

void shutdown_handler()
{
    SDL_Quit();
}

int main(int argc, char** argv)
{
    g_logFile.open("ceres.log", std::ios::trunc);

    qInstallMessageHandler(myMessageOutput);

    SDL_Init(SDL_INIT_AUDIO);

    std::atexit(shutdown_handler);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication::setOrganizationName("University of Illinois");
    QCoreApplication::setOrganizationDomain("rapp.ripe.illinois.edu");
    QCoreApplication::setApplicationName("Ceres");
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication app(argc, argv);

    cCeresSplashScreen* pSplash = new cCeresSplashScreen();
    pSplash->show();

    cMainWindow mainWin;
    mainWin.initialize(pSplash);
    mainWin.show();

    pSplash->finish(&mainWin);

    delete pSplash; 
    pSplash = nullptr;

    auto result = app.exec();

    SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return result;
}

