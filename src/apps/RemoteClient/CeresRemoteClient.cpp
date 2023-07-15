

#include <QApplication>

#include "CeresSplashScreen.hpp"
#include "RemoteClientWindow.hpp"

#include <fstream>
#include <string>
#include <chrono>

static std::ofstream g_logFile;
static std::chrono::time_point<std::chrono::system_clock> g_startTime;

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    std::chrono::time_point<std::chrono::system_clock> logTime = std::chrono::system_clock::now();
    std::chrono::duration<double> timestamp = logTime - g_startTime;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) 
    {
    case QtDebugMsg:
        g_logFile << timestamp.count() << ", ";
        g_logFile << "Debug: " << localMsg.constData();
        break;
    case QtInfoMsg:
        g_logFile << timestamp.count() << ", ";
        g_logFile << "Info: " << localMsg.constData();
        break;
    case QtWarningMsg:
        g_logFile << timestamp.count() << ", ";
        g_logFile << "Warning: " << localMsg.constData();
        break;
    case QtCriticalMsg:
        g_logFile << timestamp.count() << ", ";
        g_logFile << "Critical: " << localMsg.constData();
        break;
    case QtFatalMsg:
        g_logFile << timestamp.count() << ", ";
        g_logFile << "Fatal: " << localMsg.constData();
        break;
    default:
        return;
    }
    if (context.file)
    {
        g_logFile << " (" << context.file << ":" << context.line;
        g_logFile << ", " << context.function << ")";

    } 
    g_logFile << std::endl;
}


int main(int argc, char** argv)
{
    {
        std::string filename = "CeresRemoteClient";

        char timestamp[100] = { '\0' };
        std::time_t t = std::time(nullptr);
        std::strftime(timestamp, sizeof(timestamp), "%Y%m%d", std::localtime(&t));

        filename += "_";
        filename += timestamp;

        filename += ".log";
        g_logFile.open(filename, std::ios::app);

        std::string title = "=========== Ceres Remote Client Started ";
        std::strftime(timestamp, sizeof(timestamp), "(%H:%M:%S)", std::localtime(&t));
        title += timestamp;
        title += " ===========";

        g_logFile << title << std::endl;
        g_startTime = std::chrono::system_clock::now();
    }


    qInstallMessageHandler(myMessageOutput);

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

    try
    {
        return app.exec();
    }
    catch (const std::exception& e)
    {
        g_logFile << "Fatal Exception: " << e.what() << std::endl;
    }
}

