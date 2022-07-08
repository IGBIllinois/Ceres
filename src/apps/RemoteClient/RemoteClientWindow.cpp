
#include "RemoteClientWindow.hpp"
#include "ui_MainWindow.h"

#include "CeresSplashScreen.hpp"

#include "SensorFactory.hpp"
#include "SensorModel.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>

#include <cassert>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace
{
    std::string getCfgFilePath()
    {
        QString cfgPath;

        auto args = QApplication::arguments();
        if (args.size() > 1)
        {
            auto n = args.size() - 1;
            for (std::size_t i = 1; i < n; ++i)
            {
                QString arg = args[i];
                if (arg.compare("-c") || arg.compare("--config") || arg.compare("/c"))
                {
                    cfgPath = args[i + 1];
                    break;
                }
            }
        }

        if (!cfgPath.isEmpty())
        {
            if (QFile::exists(cfgPath))
            {
                return cfgPath.toStdString();
            }

            QString msg = "Configuration file \"";
            msg += cfgPath;
            msg += "\" does not exist!  Make sure the \"-c\" option specifies the full path to the configuration file and that the file exists.";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            return std::string();
        }

        cfgPath = QApplication::applicationDirPath();
        cfgPath += "/ceres_remote_client.json";
        if (QFile::exists(cfgPath))
            return cfgPath.toStdString();

        QString msg = "The default configuration file \"";
        msg += cfgPath;
        msg += "\" does not exist!";

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
        mb.exec();

        return std::string();
    }
}

//-----------------------------------------------------------------------------
cRemoteClientWindow::cRemoteClientWindow(QWidget* parent) :
    QMainWindow(parent),
    mpSplashScreen(nullptr),
    mpFileMenu(nullptr),
    mpHelpMenu(nullptr),
    mpUI(new Ui::MainWindow)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Remote"));

    setUnifiedTitleAndToolBarOnMac(true);

    auto cwd = std::filesystem::current_path();
    auto data_path = cwd / "Data";
    mDefaultDataPath = QString::fromLatin1(data_path.string().c_str());

    QObject::connect(&mMainModel, &cDataModel::statusMessage,  this, &cRemoteClientWindow::onStatusUpdate);
    QObject::connect(&mMainModel, &cDataModel::infoMessage,    this, &cRemoteClientWindow::onInfoMessage);
    QObject::connect(&mMainModel, &cDataModel::warningMessage, this, &cRemoteClientWindow::onWarningMessage);
    QObject::connect(&mMainModel, &cDataModel::errorMessage,   this, &cRemoteClientWindow::onErrorMessage);
}

//-----------------------------------------------------------------------------
cRemoteClientWindow::~cRemoteClientWindow()
{
    mMainModel.stopDataThread();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::initialize(cCeresSplashScreen* pSplashScreen)
{
    mpSplashScreen = pSplashScreen;
    mpSplashScreen = pSplashScreen;

    std::string cfgFileName = getCfgFilePath();
    nlohmann::json configDoc;

    if (!cfgFileName.empty())
    {
        std::ifstream in;
        in.open(cfgFileName);

        if (!in.is_open())
        {
            QString msg = "Could not open ";
            msg += cfgFileName.c_str();
            msg += " for reading!";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            exit(EXIT_FAILURE);
        }

        try
        {
            in >> configDoc;
        }
        catch (const nlohmann::json::parse_error& e)
        {
            QString msg = "Parsing error in ";
            msg += cfgFileName.c_str();
            msg += ".\n";
            msg += e.what();

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            exit(EXIT_FAILURE);
        }
        catch (const std::exception& e)
        {
            QString msg = "Unknown error in ";
            msg += cfgFileName.c_str();
            msg += ".\n";
            msg += e.what();

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            exit(EXIT_FAILURE);
        }
    }

    if (configDoc.contains("default data folder"))
    {
        auto folders = configDoc["default data folder"];
#ifdef WIN32
        if (folders.contains("windows"))
        {
            mDefaultDataPath = QString::fromLatin1(folders["windows"].get<std::string>().c_str());
        }
#endif
    }

    onStatusUpdate("Initializing menus...");
    createMainMenu();
    createSubMenusAndActions();
    createActions();

    onStatusUpdate("Initializing toolbars...");
    createToolBars();

    onStatusUpdate("Initializing status bar...");
    createStatusBar();

    try
    {
        onStatusUpdate("Initializing sensors...");
        createSensorModelsAndViews(configDoc);

        onStatusUpdate("Initializing TCP server...");
        if (!initializeServer(configDoc))
        {
            QMessageBox mb(QMessageBox::Critical, "TCP Server Error", "Could not start the TCP server!");
            mb.exec();

            exit(EXIT_FAILURE);
        }
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error in ";
        msg += cfgFileName;
        msg += ": ";
        msg += e.what();

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
        mb.exec();

        exit(EXIT_FAILURE);
    }

    mpSplashScreen = nullptr;

    mMainModel.startDataThread();
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::onStatusUpdate(QString msg)
{
    if (mpSplashScreen)
    {
        mpSplashScreen->showMessage(msg);
        return;
    }

    if (statusBar())
        statusBar()->showMessage(msg);
}

void cRemoteClientWindow::onInfoMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Information, title, msg);
    msg_box.exec();
}

void cRemoteClientWindow::onWarningMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Warning, title, msg);
    msg_box.exec();
}

void cRemoteClientWindow::onErrorMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Critical, title, msg);
    msg_box.exec();
}


//-----------------------------------------------------------------------------
void cRemoteClientWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Menu
    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);

    // Build the Help Menu
//    pMenuItem = new QAction(tr("&About"), this);
//    connect(pMenuItem, &QAction::triggered, this, &cRemoteClientWindow::helpAbout);
//    mpHelpMenu->addAction(pMenuItem);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createActions()
{

}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createToolBars()
{
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createStatusBar()
{
    statusBar();
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createSensorModelsAndViews(const nlohmann::json& configDoc)
{
    auto sensors = configDoc["sensors"];

    for (auto sensor : sensors)
    {
        std::string type = sensor["type"];
        auto widgets = create_sensor(type, sensor, true);

        if (widgets.pModel == nullptr)
        {
            std::string msg = "Unknown sensor type \"";
            msg += type;
            msg += "\".";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
            mb.exec();
            continue;
        }

        QObject::connect(widgets.pModel, &cSensorModel::statusMessage,  this, &cRemoteClientWindow::onStatusUpdate);
        QObject::connect(widgets.pModel, &cSensorModel::infoMessage,    this, &cRemoteClientWindow::onInfoMessage);
        QObject::connect(widgets.pModel, &cSensorModel::warningMessage, this, &cRemoteClientWindow::onWarningMessage);
        QObject::connect(widgets.pModel, &cSensorModel::errorMessage,   this, &cRemoteClientWindow::onErrorMessage);

        if (configDoc.contains(type))
        {
            bool validSensor = false;
            try
            {
                validSensor = widgets.pModel->configure(configDoc[type]);
            }
            catch (const std::exception& e)
            {
                validSensor = false;
            }
        }

        mMainModel.addSensor(widgets.pModel);

        if (widgets.pStatusBar)
        {
            statusBar()->addPermanentWidget(widgets.pStatusBar);
        }
    }
}

//-----------------------------------------------------------------------------
bool cRemoteClientWindow::initializeServer(const nlohmann::json& configDoc)
{
    auto serverInfo = configDoc["server"];
    std::string ip = serverInfo["ip"];
    uint16_t port = serverInfo["port"];

    return mMainModel.startTcpServer(ip, port);
}
