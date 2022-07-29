

#include <QApplication>

#include "RemoteClientWindow.hpp"

#include <fstream>
#include <string>
#include <chrono>

static std::ofstream logFile;
static std::chrono::time_point<std::chrono::system_clock> startTime;

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    std::chrono::time_point<std::chrono::system_clock> logTime = std::chrono::system_clock::now();
    std::chrono::duration<double> timestamp = logTime - startTime;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) 
    {
    case QtDebugMsg:
        logFile << timestamp.count() << ", ";
        logFile << "Debug: " << localMsg.constData();
        break;
    case QtInfoMsg:
        logFile << timestamp.count() << ", ";
        logFile << "Info: " << localMsg.constData();
        break;
    case QtWarningMsg:
        logFile << timestamp.count() << ", ";
        logFile << "Warning: " << localMsg.constData();
        break;
    case QtCriticalMsg:
        logFile << timestamp.count() << ", ";
        logFile << "Critical: " << localMsg.constData();
        break;
    case QtFatalMsg:
        logFile << timestamp.count() << ", ";
        logFile << "Fatal: " << localMsg.constData();
        break;
    default:
        return;
    }
    if (context.file)
    {
        logFile << " (" << context.file << ":" << context.line;
        logFile << ", " << context.function << ")";

    } 
    logFile << std::endl;
}


int main(int argc, char** argv)
{
    logFile.open("CeresRemoteClient.log", std::ios::app);
    logFile << "=========== Ceres Remote Client Started ===========" << std::endl;
    startTime = std::chrono::system_clock::now();


    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);

    cCeresSplashScreen* pSplash = nullptr;

    cRemoteClientWindow mainWin;
    mainWin.initialize(pSplash);
    mainWin.show();

    return app.exec();
}

