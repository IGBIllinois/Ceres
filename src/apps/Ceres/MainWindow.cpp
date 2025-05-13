
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "HobbsMeter.hpp"

#include "CeresSplashScreen.hpp"

#include "CtrlDataModel.hpp"
#include "CtrlDataModelLocal.hpp"
#include "CtrlDataModelRemote.hpp"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"
#include "ExperimentToolbar.hpp"
#include "BatchExpConfirmDlg.hpp"

#include "ExperimentCtrlFactory.hpp"
#include "ExperimentCtrlView.hpp"
#include "ExperimentCtrlModel.hpp"

#include "SensorFactory.hpp"
#include "SensorModel.hpp"
#include "SensorPropertyPage.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QSound>


#include <cassert>
#include <sstream>
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
        cfgPath += "/ceres.json";
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
cMainWindow::cMainWindow(QWidget* parent) :
    QMainWindow(parent),
    mpSplashScreen(nullptr),
    mpExperiments(nullptr),
    mpExpLoad(nullptr),
    mpExpRun(nullptr),
    mpExpPause(nullptr),
    mpExpStop(nullptr),
    mpFileMenu(nullptr),
    mpExperimentMenu(nullptr),
    mpViewMenu(nullptr),
    mpHelpMenu(nullptr),
    mpFileBar(nullptr),
    mpHobbsMeter(nullptr),
    mpUI(new Ui::MainWindow),
    mpModel(nullptr),
    mpController(nullptr)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres"));

    setUnifiedTitleAndToolBarOnMac(true);

    auto cwd = std::filesystem::current_path();
    auto data_path = cwd / "Data";
    auto exp_path = cwd / "Experiments";
    mDefaultDataPath = QString::fromLatin1(data_path.string().c_str());
    mExperimentFilesPath = QString::fromLatin1(exp_path.string().c_str());
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    mpModel->stopDataThread();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize(cCeresSplashScreen* pSplashScreen)
{
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
        onStatusUpdate("Initializing dock windows...");
        createDockWindows(configDoc);

        onStatusUpdate("Initializing data model...");
        createDataModel(configDoc);

        onStatusUpdate("Initializing experiment controller...");
        createExperimentController(configDoc);

        onStatusUpdate("Initializing sensors...");
        createSensorModelsAndViews(configDoc);
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

    statusBar()->addPermanentWidget(mpHobbsMeter);

    QTimer::singleShot(1000, mpModel, &cDataModel::startDataThread);
}

//-----------------------------------------------------------------------------
void cMainWindow::fileRefresh()
{
    mpExperiments->refresh();

    emit refreshDisplay();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void cMainWindow::fileAddExperiment()
{
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentLoad()
{
    mBatchFileName.clear();

    if (mpModel->isExperimentRunning())
    {
        //TODO: Something here!
    }

    auto* pExperiment = static_cast<cExperimentTreeItem*>(mpExperiments->currentItem());

    if (pExperiment == nullptr)
    {
        return;
    }

    if (pExperiment->hasExperimentDocument())
    {
        bool result = loadExperiment(*pExperiment);
        return;
    }

    if (0 == pExperiment->childCount())
    {
        return;
    }

    cBatchExpConfirmDlg* pDlg = new cBatchExpConfirmDlg(this);

    pDlg->initialize(pExperiment);

    auto result = pDlg->exec();
    if (result == QDialog::Rejected)
        return;

    mBatchProcess = pDlg->getSelectedExperiments();

    if (mBatchProcess.empty())
        return;

    auto expFile = mBatchProcess.front();
    mBatchProcess.erase(mBatchProcess.begin());

    loadExperiment(expFile);
}

//-----------------------------------------------------------------------------
bool cMainWindow::loadExperiment(const cExperimentTreeItem& experiment)
{
    QString msg = "Loading experiment \"";
    msg += experiment.text(0);
    msg += "\" from file ";
    msg += experiment.getFilename();
    onStatusUpdate(msg);

    std::string name = experiment.text(0).toStdString();
    auto expDoc = experiment.getExperimentDocument();
    if (!mpModel->loadExperiment(name, expDoc))
    {
        return false;
    }

    return true;
}

bool cMainWindow::loadExperiment(const std::filesystem::path& experiment_file)
{
    using namespace nlohmann;

    std::ifstream in;
    in.open(experiment_file);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc;
    std::string name;

    try
    {
        in >> jsonDoc;

        if (jsonDoc.contains("experiment name"))
            name = jsonDoc["experiment name"];
        else
            name = jsonDoc["experiment_name"];
    }
    catch (const detail::exception& e)
    {
        QString msg = "Failed to loading experiment file: ";
        msg += QString::fromStdString(experiment_file.string());
        onWarningMessage("File Error", msg);

        return false;
    }


    QString msg = "Loading experiment \"";
    msg += QString::fromStdString(name);
    msg += "\" from file ";
    msg += QString::fromStdString(experiment_file.string());
    onStatusUpdate(msg);

    if (!mpModel->loadExperiment(name, jsonDoc))
    {
        QString msg = "Experiment \"";
        msg += QString::fromStdString(name);
        msg += "\" from file ";
        msg += QString::fromStdString(experiment_file.string());
        msg += " failed to load!";
        onStatusUpdate(msg);
        return false;
    }

    mBatchFileName = name;

    return true;
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentRun()
{
    if (mpModel->isExperimentRunning())
    {
        if (mpModel->isExperimentPaused())
        {
            mpModel->startExperiment();

            emit setExperimentActions(false, false, true, true);

        }
        return;
    }

    if (!mpModel->systemReady())
    {
        return;
    }

    // Reload the experiment each time incase the experiment was tweaked
    if (mBatchFileName.empty())
    {
        onExperimentLoad();
    }

    if (!mpModel->isExperimentLoaded())
    {
        return;
    }

    if (mpModel->experimentRequiresDataFile())
    {
        if (mBatchFileName.empty())
        {
            std::string fileName = mpModel->experimentTitle();
            if (!mpModel->openDataFile(mDefaultDataPath, fileName, false))
            {
                mpModel->terminateExperiment();
                return;
            }
        }
        else
        {
            if (!mpModel->openDataFile(mDefaultDataPath, mBatchFileName, true))
            {
                mpModel->terminateExperiment();
                return;
            }
        }
    }

    mpModel->startExperiment();

    emit setExperimentActions(false, false, true, true);

    emit experimentRunning();
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentPause()
{
    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    mpModel->pauseExperiment();

    emit setExperimentActions(false, true, true, true);

    emit experimentPaused();
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentStop()
{
    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    mpModel->terminateExperiment();

//    onExperimentTerminated();
}

void cMainWindow::onSetExperimentActions(bool load, bool run, bool pause, bool stop)
{
    mpExpLoad->setEnabled(load);
    mpExpRun->setEnabled(run);
    mpExpPause->setEnabled(pause);
    mpExpStop->setEnabled(stop);
}

//-----------------------------------------------------------------------------
void cMainWindow::helpAbout()
{

}

void cMainWindow::onStatusUpdate(QString msg)
{
    if (mpSplashScreen)
    {
        mpSplashScreen->showMessage(msg);
        return;
    }

    if (statusBar())
        statusBar()->showMessage(msg);
}

void cMainWindow::onInfoMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Information, title, msg);
    msg_box.exec();
}

void cMainWindow::onWarningMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Warning, title, msg);
    msg_box.exec();
}

void cMainWindow::onErrorMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Critical, title, msg);
    msg_box.exec();
}

void cMainWindow::onLogMessage(uint8_t type, QString device, QString msg)
{
    switch (type)
    {
    case logINFO:
        onInfoMessage(device, msg);
        break;
    case logWARNING:
        onWarningMessage(device, msg);
        break;
    case logERROR:
        onErrorMessage(device, msg);
        break;
    }

    onStatusUpdate(msg);
}

void cMainWindow::onExperimentTerminated()
{
    mBatchFileName.clear();
    mBatchProcess.clear();

    emit setExperimentActions(true, true, false, false);

    emit experimentStopped();

    emit showMessage("Experiment stopped!");
}

void cMainWindow::onExperimentCompleted()
{
    mBatchFileName.clear();

    while (!mBatchProcess.empty())
    {
        auto expFile = mBatchProcess.front();
        mBatchProcess.erase(mBatchProcess.begin());

        if (loadExperiment(expFile))
        {
            onExperimentRun();
            return;
        }
    }

    emit setExperimentActions(true, true, false, false);

    emit experimentStopped();

    QSound::play(":/ripe.illinois.edu/end_experiment.wav");
    onStatusUpdate("Experiment completed!");
}

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpExperimentMenu = mpUI->menuBar->addMenu(tr("&Experiment"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpSensorMenu = mpUI->menuBar->addMenu(tr("&Sensors"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Menu
    pMenuItem = new QAction(tr("Refresh"), this);
    pMenuItem->setStatusTip(tr("Refresh the experiment window"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::fileRefresh);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);

    // Build the Experiment Menu
    mpExpLoad = new QAction(tr("&Load"), this);
    mpExpLoad->setStatusTip(tr("Load experiment..."));
    connect(mpExpLoad, &QAction::triggered, this, &cMainWindow::onExperimentLoad);
    mpExperimentMenu->addAction(mpExpLoad);

    mpExperimentMenu->addSeparator();

    mpExpRun = new QAction(tr("&Run"), this);
    mpExpRun->setStatusTip(tr("Run experiment..."));
    connect(mpExpRun, &QAction::triggered, this, &cMainWindow::onExperimentRun);
    mpExperimentMenu->addAction(mpExpRun);

    mpExpPause = new QAction(tr("&Pause"), this);
    mpExpPause->setStatusTip(tr("Pause the currently running experiment"));
    connect(mpExpPause, &QAction::triggered, this, &cMainWindow::onExperimentPause);
    mpExperimentMenu->addAction(mpExpPause);
    mpExpPause->setEnabled(false);

    mpExpStop = new QAction(tr("&Stop"), this);
    mpExpStop->setStatusTip(tr("Stop the currently running experiment"));
    connect(mpExpStop, &QAction::triggered, this, &cMainWindow::onExperimentStop);
    mpExperimentMenu->addAction(mpExpStop);
    mpExpStop->setEnabled(false);

    connect(this, &cMainWindow::setExperimentActions, this, &cMainWindow::onSetExperimentActions);

    // Build the View Menu
    /* The view menu is built by the dock window system */

    // Build the Sensor Menu
    /* The sensor menu is built by the sensor loading system */

    // Build the Help Menu
    pMenuItem = new QAction(tr("&About"), this);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::helpAbout);
    mpHelpMenu->addAction(pMenuItem);
}

//-----------------------------------------------------------------------------
void cMainWindow::createActions()
{

}

//-----------------------------------------------------------------------------
void cMainWindow::createToolBars()
{
//    mpFileBar = addToolBar("File");

    auto* toolbar = new cExperimentToolbar(this);
    connect(this, &cMainWindow::experimentRunning, toolbar, &cExperimentToolbar::experimentRunning);
    connect(this, &cMainWindow::experimentPaused, toolbar, &cExperimentToolbar::experimentPaused);
    connect(this, &cMainWindow::experimentStopped, toolbar, &cExperimentToolbar::experimentStopped);

    connect(toolbar, &cExperimentToolbar::loadSelected, this, &cMainWindow::onExperimentLoad);
    connect(toolbar, &cExperimentToolbar::runSelected, this, &cMainWindow::onExperimentRun);
    connect(toolbar, &cExperimentToolbar::pauseSelected, this, &cMainWindow::onExperimentPause);
    connect(toolbar, &cExperimentToolbar::stopSelected, this, &cMainWindow::onExperimentStop);

    addToolBar(toolbar);
}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();

    connect(this, &cMainWindow::showMessage, statusBar(), &QStatusBar::showMessage);

    mpHobbsMeter = new cHobbsMeter(statusBar());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows(const nlohmann::json& configDoc)
{
    if (configDoc.contains("experiment file folder"))
    {
        auto folders = configDoc["experiment file folder"];
#ifdef WIN32
        if (folders.contains("windows"))
        {
            mExperimentFilesPath = QString::fromLatin1(folders["windows"].get<std::string>().c_str());
        }
#endif
    }

    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new cExperimentManager(mExperimentFilesPath, dock);
    connect(mpExperiments, &cExperimentManager::runExperiment, this, &cMainWindow::onExperimentRun);


    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDataModel(const nlohmann::json& configDoc)
{
    auto data_model = configDoc["data model"];

    if (data_model.is_string())
    {
        std::string type = data_model.get<std::string>();

        if (type == "local")
            mpModel = new cCtrlDataModelLocal(this);
        else
        {
            QString msg = "Unknown data model: ";
            msg += type.c_str();

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            exit(EXIT_FAILURE);
        }

        QObject::connect(mpModel, &cCtrlDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
        QObject::connect(mpModel, &cCtrlDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
        QObject::connect(mpModel, &cCtrlDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
        QObject::connect(mpModel, &cCtrlDataModel::errorMessage, this, &cMainWindow::onErrorMessage);
    }
    else if (data_model.is_object())
    {
        std::string c4_ip = data_model["c4_ip"];
        uint16_t port = data_model["port"];

        std::string c3_ip;
        if (data_model.contains("c3_ip"))
            c3_ip = data_model["c3_ip"];

        cCtrlDataModelRemote* pModel = new cCtrlDataModelRemote(this);

        QObject::connect(pModel, &cDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
        QObject::connect(pModel, &cDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
        QObject::connect(pModel, &cDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
        QObject::connect(pModel, &cDataModel::errorMessage, this, &cMainWindow::onErrorMessage);

        QObject::connect(pModel, &cCtrlDataModelRemote::addSensorPropertyPage,
                        this, &cMainWindow::addSensorPropertyPage);
        QObject::connect(pModel, &cCtrlDataModelRemote::removeSensorPropertyPage,
                        this, &cMainWindow::removeSensorPropertyPage);

        auto* dockWidget = new QDockWidget();
        pModel->createView(dockWidget);

        if (dockWidget->widget() != nullptr)
        {
            dockWidget->setParent(this);

            addDockWidget(Qt::RightDockWidgetArea, dockWidget);
            mpViewMenu->addAction(dockWidget->toggleViewAction());
        }

        bool result = pModel->try_to_connect(QString(c4_ip.c_str()), port,
            false, QString(c3_ip.c_str()));

        if (!result)
        { }

        mpModel = pModel;
    }

    QObject::connect(mpModel, &cCtrlDataModel::experimentTerminated, this, &cMainWindow::onExperimentTerminated);
    QObject::connect(mpModel, &cCtrlDataModel::experimentCompleted, this, &cMainWindow::onExperimentCompleted);
}

//-----------------------------------------------------------------------------
void cMainWindow::createExperimentController(const nlohmann::json& configDoc)
{
    std::string name;
    nlohmann::json ctrlDoc;

    if (configDoc["controller"].is_object())
    {
        ctrlDoc = configDoc["controller"];
        name = ctrlDoc["type"];
    }
    else
    {
        name = configDoc["controller"];
    }

    sExperimentControllerWidgets widgets = create_experiment_controller(name, ctrlDoc);

    cExperimentControlModel* pModel = widgets.pModel;
    mpController = widgets.pView;

    if ((pModel == nullptr) || (mpController == nullptr))
    {
        //continue;
    }

    QObject::connect(pModel, &cExperimentControlModel::updateControllerConnection,
        mpHobbsMeter, &cHobbsMeter::updateControllerConnection);

    QObject::connect(pModel, &cExperimentControlModel::experimentStateChanged,
        mpHobbsMeter, &cHobbsMeter::onExperimentStateChange);

    QObject::connect(pModel, &cExperimentControlModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(pModel, &cExperimentControlModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(pModel, &cExperimentControlModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(pModel, &cExperimentControlModel::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::connect(mpController, &cExperimentControlView::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(mpController, &cExperimentControlView::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(mpController, &cExperimentControlView::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(mpController, &cExperimentControlView::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::connect(this, &cMainWindow::refreshDisplay, mpController, &cExperimentControlView::refresh);

    QObject::connect(pModel, &cExperimentControlModel::experimentStatus,
        mpController, &cExperimentControlView::experimentStatusUpdating);

    QObject::connect(pModel, &cExperimentControlModel::experimentStateChanged,
        mpController, &cExperimentControlView::experimentStateChanging);

    mpModel->addExperimentControlModel(pModel);

    if (configDoc.contains(name))
    {
        pModel->configure(configDoc[name]);
        mpController->configure(configDoc[name]);
    }

    setCentralWidget(mpController);

    if (widgets.pDockableView)
    {
        widgets.pDockableView->setParent(this);
        widgets.pDockableView->setAllowedAreas(Qt::AllDockWidgetAreas);
        widgets.pDockableView->hide();

        addDockWidget(Qt::BottomDockWidgetArea, widgets.pDockableView);
        mpViewMenu->addAction(widgets.pDockableView->toggleViewAction());
    }

    if (widgets.pStatusBar)
    {
        statusBar()->addPermanentWidget(widgets.pStatusBar);
    }

    if (widgets.pToolBar)
    {
        addToolBar(widgets.pToolBar);
    }
}

//-----------------------------------------------------------------------------
void cMainWindow::createSensorModelsAndViews(const nlohmann::json& configDoc)
{
    auto sensors = configDoc["sensors"];

    for (auto sensor : sensors)
    {
        std::string type = sensor["type"];
        auto widgets = create_sensor(type, sensor);

        if (widgets.pModel == nullptr)
        {
            std::string msg = "Unknown sensor type or sensor not found: type is \"";
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

        QObject::connect(widgets.pModel, &cSensorModel::statusMessage, this, &cMainWindow::onStatusUpdate);
//        QObject::connect(widgets.pModel, &cSensorModel::infoMessage, this, &cMainWindow::onInfoMessage);
//        QObject::connect(widgets.pModel, &cSensorModel::warningMessage, this, &cMainWindow::onWarningMessage);
//        QObject::connect(widgets.pModel, &cSensorModel::errorMessage, this, &cMainWindow::onErrorMessage);
        QObject::connect(widgets.pModel, &cSensorModel::logMessage, this, &cMainWindow::onLogMessage);

        if (!configDoc.contains(type))
        {
            QString msg = "The follow object is missing from the configuration file: ";
            msg += QString::fromStdString(type);
            onErrorMessage("Configuration Error", msg);
            continue;
        }

        bool validSensor = false;
        try
        {
            std::string entry = type;
            if (sensor.contains("instance"))
            {
                entry += ":";
                entry += sensor["instance"];
            }

            validSensor = widgets.pModel->configure(configDoc[entry]);
        }
        catch (const std::exception& e)
        {
            validSensor = false;
        }

        if (!validSensor)
        {
            remove_sensor(type, widgets);
            continue;
        }

        mpModel->addSensor(widgets.pModel);

        if (widgets.pDockableView)
        {
            widgets.pDockableView->setParent(this);

            addDockWidget(Qt::RightDockWidgetArea, widgets.pDockableView);
            mpViewMenu->addAction(widgets.pDockableView->toggleViewAction());
        }

        if (widgets.pStatusBar)
        {
            statusBar()->addPermanentWidget(widgets.pStatusBar);
        }

        if (widgets.pToolBar)
        {
            addToolBar(widgets.pToolBar);
        }

        if (widgets.pPropertyPage)
        {
            widgets.pPropertyPage->createWidgets();
            widgets.pPropertyPage->doLayout();
            mpSensorMenu->addAction(widgets.pPropertyPage->showAction());
        }
    }
}

void cMainWindow::addSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->addAction(pAction);
}

void cMainWindow::removeSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->removeAction(pAction);
}
