

#include <QApplication>

#include "../common/CeresSplashScreen.hpp"
#include "RemoteClientWindow.hpp"

#include <windows.h>
#include <eh.h>
#include <stdexcept>

#include <fstream>

static std::ofstream logFile;

class SE_Exception : public std::exception
{
private:
    const unsigned int nSE;
public:
    SE_Exception() noexcept : SE_Exception{ 0 } {}
    SE_Exception(unsigned int n) noexcept : nSE{ n } {}
    unsigned int getSeNumber() const noexcept { return nSE; }
};

class Scoped_SE_Translator
{
private:
    const _se_translator_function old_SE_translator;
public:
    Scoped_SE_Translator(_se_translator_function new_SE_translator) noexcept
        : old_SE_translator{ _set_se_translator(new_SE_translator) } {}
    ~Scoped_SE_Translator() noexcept { _set_se_translator(old_SE_translator); }
};

void trans_func(unsigned int u, EXCEPTION_POINTERS*)
{
    throw SE_Exception(u);
}

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
    Scoped_SE_Translator scoped_se_translator{ trans_func };

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

    try
    {
        return app.exec();
    }
    catch (const SE_Exception& e)
    {
        logFile << e.getSeNumber() << std::endl;
    }
    catch (const std::exception& e)
    {
        logFile << e.what() << std::endl;
    }

    return 0;
}

