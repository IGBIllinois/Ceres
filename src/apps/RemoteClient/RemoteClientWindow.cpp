
#include "RemoteClientWindow.hpp"
#include "ui_MainWindow.h"

#include "CeresSplashScreen.hpp"
#include "RemoteClientCentralWindow.hpp"

#include "SensorFactory.hpp"
#include "SensorModel.hpp"
#include "SensorStatusView.hpp"

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
    mpUI(new Ui::MainWindow),
    mpCentralWindow(nullptr)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Remote"));

    setUnifiedTitleAndToolBarOnMac(true);

    auto cwd = std::filesystem::current_path();
    auto data_path = cwd / "Data";
    mMainModel.setDefaultDataPath(data_path.string());

    mpCentralWindow = new cRemoteClientCentalWindow(this);

    QObject::connect(&mMainModel, &cDataModel::statusMessage,  this, &cRemoteClientWindow::onStatusUpdate);
    QObject::connect(&mMainModel, &cDataModel::infoMessage,    this, &cRemoteClientWindow::onInfoMessage);
    QObject::connect(&mMainModel, &cDataModel::warningMessage, this, &cRemoteClientWindow::onWarningMessage);
    QObject::connect(&mMainModel, &cDataModel::errorMessage,   this, &cRemoteClientWindow::onErrorMessage);

    setCentralWidget(mpCentralWindow);
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
    std::string cfgFileName;
    nlohmann::json configDoc;

    if (mpSplashScreen)
    {
        cfgFileName = getCfgFilePath();

        qInfo() << "Loading configuration file " << cfgFileName.c_str() << "...";

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
                configDoc = nlohmann::json::parse(in, nullptr, true, true);
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
                mMainModel.setDefaultDataPath(folders["windows"].get<std::string>());
            }
#endif
        }
    }

    createMainMenu();
    createSubMenusAndActions();
    createActions();

    createToolBars();

    createStatusBar();

    if (mpSplashScreen)
    {
        try
        {
            qInfo() << "Initializing sensors...";

            onStatusUpdate("Initializing sensors...");
            createSensorModelsAndViews(configDoc);

            qInfo() << "Initializing TCP server...";

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
    }

    mpSplashScreen = nullptr;

    qInfo() << "Single shot timer to start data acquisition.";
    QTimer::singleShot(1000, this, &cRemoteClientWindow::startDataAcquisitionSystem);
}

void cRemoteClientWindow::startDataAcquisitionSystem()
{
    if (mMainModel.sensorCount() == 0)
    {
        std::string cfgFileName = getCfgFilePath();
        nlohmann::json configDoc;

        qInfo() << "Loading configuration file " << cfgFileName.c_str() << "...";

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
                configDoc = nlohmann::json::parse(in, nullptr, true, true);
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
                mMainModel.setDefaultDataPath(folders["windows"].get<std::string>());
            }
#endif
        }

        try
        {
            qInfo() << "Initializing sensors...";

            onStatusUpdate("Initializing sensors...");
            createSensorModelsAndViews(configDoc);

            qInfo() << "Initializing TCP server...";

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
    }

    qInfo() << "Starting data acquisition thread....";
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

    mMainModel.sendStatusMessage(msg);
}

void cRemoteClientWindow::onInfoMessage(QString title, QString msg)
{
    qInfo() << title << ": " << msg;
    QMessageBox msg_box(QMessageBox::Information, title, msg);
    msg_box.exec();
}

void cRemoteClientWindow::onWarningMessage(QString title, QString msg) const
{
    qWarning() << title << ": " << msg;
    QMessageBox msg_box(QMessageBox::Warning, title, msg);
    msg_box.exec();
}

void cRemoteClientWindow::onErrorMessage(QString title, QString msg)
{
    qCritical() << title << ": " << msg;
    QMessageBox msg_box(QMessageBox::Critical, title, msg);
    msg_box.exec();
}

void cRemoteClientWindow::onLogMessage(uint8_t type, QString device, QString msg)
{
    mMainModel.sendLogMessage(type, device, msg);

    if (mpCentralWindow)
        mpCentralWindow->logMessage(type, device, msg);
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
        qInfo() << "Creating sensor: " << type.c_str();

        auto widgets = create_sensor(type, sensor, true);

        if (widgets.pModel == nullptr)
        {
            std::string msg = "Unknown sensor type \"";
            msg += type;
            msg += "\"";

            if (sensor.contains("sensor"))
            {
                msg += ", sensor name: ";
                msg += sensor["sensor"];
            }
            else
                msg += ".";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
            mb.exec();
            continue;
        }

        QObject::connect(widgets.pModel, &cSensorModel::statusMessage,  this, &cRemoteClientWindow::onStatusUpdate);
        QObject::connect(widgets.pModel, &cSensorModel::logMessage,     this, &cRemoteClientWindow::onLogMessage);

        if (widgets.pStatusBar)
        {
            statusBar()->addPermanentWidget(widgets.pStatusBar);
        }

        if (widgets.pController)
        {
            mMainModel.addSensorController(widgets.pController);
        }

        if (widgets.pRemoteStatusView)
        {
            mpCentralWindow->addTab(widgets.pRemoteStatusView, widgets.pRemoteStatusView->windowTitle());
            QObject::connect(widgets.pModel, &cSensorModel::sensorNameChanging, mpCentralWindow, &cRemoteClientCentalWindow::updateSensorName);

            widgets.pModel->updateViews();
        }

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
