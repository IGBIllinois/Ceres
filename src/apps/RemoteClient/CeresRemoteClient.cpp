

#include <QApplication>

#include "../common/CeresSplashScreen.hpp"
#include "RemoteClientWindow.hpp"


#include <fstream>

static std::ofstream logFile;


void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char* file = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        logFile << "Debug: " << localMsg.constData() << " (" << file;
        logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtInfoMsg:
        logFile << "Info: " << localMsg.constData() << " (" << file;
        logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtWarningMsg:
        logFile << "Warning: " << localMsg.constData() << " (" << file;
        logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtCriticalMsg:
        logFile << "Critical: " << localMsg.constData() << " (" << file;
        logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    case QtFatalMsg:
        logFile << "Fatal: " << localMsg.constData() << " (" << file;
        logFile << ":" << context.line << ", " << function << ")" << std::endl;
        break;
    }
}


int main(int argc, char** argv)
{
    logFile.open("CeresRemoteClient.log", std::ios::trunc);

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

	return app.exec();
}

