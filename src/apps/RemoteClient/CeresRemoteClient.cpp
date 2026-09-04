

#include <QApplication>
#include <QStyleFactory>

#include "CeresSplashScreen.hpp"
#include "RemoteClientWindow.hpp"

#include <QSharedMemory>

#include <fstream>
#include <string>
#include <chrono>
#include <exception>
#include <stdexcept>


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

void on_terminate()
{
    try 
    {
        auto ex = std::current_exception();
        if (ex) 
            std::rethrow_exception(ex);
    }
    catch (const std::exception& e) 
    {
        g_logFile << "Unhandled exception: " << e.what() << "\n";
    }
    catch (...) 
    {
        g_logFile << "Unhandled unknown exception\n";
    }

    g_logFile.flush();

    std::abort();
}

int main(int argc, char** argv)
{
    QSharedMemory thereCanBeOnlyOne("There Can Only Be One");

    if (!thereCanBeOnlyOne.create(128))
    {
        return -1;
    }

    auto termination_handler = std::set_terminate(on_terminate);

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
    QApplication::setStyle(QStyleFactory::create("Fusion"));

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
        g_logFile.flush();
    }

    std::set_terminate(termination_handler);

    thereCanBeOnlyOne.deleteLater();
}

