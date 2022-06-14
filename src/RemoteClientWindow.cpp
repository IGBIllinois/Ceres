
#include "RemoteClientWindow.hpp"
#include "ui_MainWindow.h"

#include "CeresSplashScreen.hpp"

#include "SensorFactory.hpp"
#include "Sensors/SensorModel.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>


#include <cassert>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>

namespace
{
    std::string getCfgFilePath()
    {
 //       std::string cfgFilePath = "C:/igb/Ceres/build/bin/Debug/ceres.json";
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
//        cfgPath = QDir::currentPath();
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
cRemoteClientWindow::cRemoteClientWindow(QWidget* parent) :
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
    mpUI(new Ui::MainWindow),
    mpController(nullptr)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres"));

    setUnifiedTitleAndToolBarOnMac(true);

    QObject::connect(&mMainModel, &cDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(&mMainModel, &cDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(&mMainModel, &cDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(&mMainModel, &cDataModel::errorMessage, this, &cMainWindow::onErrorMessage);
    QObject::connect(&mMainModel, &cDataModel::experimentCompleted, this, &cMainWindow::onExperimentCompleted);
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

    onStatusUpdate("Initializing menus...");
    createMainMenu();
    createSubMenusAndActions();
    createActions();

    onStatusUpdate("Initializing toolbars...");
    createToolBars();

    onStatusUpdate("Initializing status bar...");
    createStatusBar();

    onStatusUpdate("Initializing dock windows...");
    createDockWindows();

    onStatusUpdate("Initializing experiment controller...");
    if (!createExperimentController())
    {
        exit(EXIT_FAILURE);
    }

    onStatusUpdate("Initializing sensors...");
    createSensorModelsAndViews();

    mpSplashScreen = nullptr;

    mMainModel.startDataThread();
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::fileNew()
{
    if (mMainModel.isExperimentRunning())
    {
        //TODO: Something here!
    }

    auto* pExperiment = static_cast<cExperimentTreeItem*>(mpExperiments->currentItem());
    if ((pExperiment == nullptr) || ( ! pExperiment->hasExperimentDocument()))
    {
        return;
    }

    QString msg = "Loading experiment \"";
    msg += pExperiment->text(0);
    msg += "\" from file ";
    msg += pExperiment->getFilename();

    onStatusUpdate(msg);
    auto expDoc = pExperiment->getExperimentDocument();
    if (!mMainModel.loadExperiment(expDoc))
    {
    }

}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::fileAddExperiment()
{
    QTreeWidgetItem* experiment = new QTreeWidgetItem();
    experiment->setText(0, "Hello");
    QTreeWidgetItem* title = new QTreeWidgetItem();
    title->setText(0, "Title");
    experiment->addChild(title);
    mpExperiments->addTopLevelItem(experiment);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::experimentLoad()
{
    if (mMainModel.isExperimentRunning())
    {
        //TODO: Something here!
    }

    auto* pExperiment = static_cast<cExperimentTreeItem*>(mpExperiments->currentItem());
    if ((pExperiment == nullptr) || (!pExperiment->hasExperimentDocument()))
    {
        cExperimentSelectDlg* dlg = new cExperimentSelectDlg(this);
        dlg->initialize(mpExperiments);
        auto result = dlg->exec();
        if (result == QDialog::Rejected)
            return;
        
        pExperiment = dlg->currentItem();
        if ((pExperiment == nullptr) || (!pExperiment->hasExperimentDocument()))
        {
            //TODO: Something here!
            return;
        }
    }

    QString msg = "Loading experiment \"";
    msg += pExperiment->text(0);
    msg += "\" from file ";
    msg += pExperiment->getFilename();

    onStatusUpdate(msg);
    auto expDoc = pExperiment->getExperimentDocument();
    if (!mMainModel.loadExperiment(expDoc))
    {
    }
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::experimentRun()
{
    if (mMainModel.isExperimentRunning())
    {
        if (mMainModel.isExperimentPaused())
        {
            mMainModel.startExperiment();

            mpExpLoad->setEnabled(false);
            mpExpRun->setEnabled(false);
            mpExpPause->setEnabled(true);
            mpExpStop->setEnabled(true);
        }
        return;
    }

    if (!mMainModel.isExperimentLoaded())
    {
        experimentLoad();

        if (!mMainModel.isExperimentLoaded())
        {
            return;
        }
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), QString(), tr("Ceres data (*.ceres);;All Files (*.*)"));

    if (fileName.isEmpty())
        return;

    if (!mMainModel.openDataFile(fileName.toStdString()))
        return;

    QString msg = "Running experiment: ";
    msg += mMainModel.experimentTitle().c_str();

    onStatusUpdate(msg);

    mMainModel.startExperiment();

    mpExpLoad->setEnabled(false);
    mpExpRun->setEnabled(false);
    mpExpPause->setEnabled(true);
    mpExpStop->setEnabled(true);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::experimentPause()
{
    if (!mMainModel.isExperimentRunning())
    {
        return;
    }

    mMainModel.pauseExperiment();

    mpExpLoad->setEnabled(false);
    mpExpRun->setEnabled(true);
    mpExpPause->setEnabled(false);
    mpExpStop->setEnabled(true);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::experimentStop()
{
    if (!mMainModel.isExperimentRunning())
    {
        return;
    }

    mMainModel.terminateExperiment();

    onExperimentCompleted();
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::helpAbout()
{

}

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


void cRemoteClientWindow::onExperimentCompleted()
{
    mpExpLoad->setEnabled(true);
    mpExpRun->setEnabled(true);
    mpExpPause->setEnabled(false);
    mpExpStop->setEnabled(false);
}


//-----------------------------------------------------------------------------
void cRemoteClientWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpExperimentMenu = mpUI->menuBar->addMenu(tr("&Experiment"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Menu
    pMenuItem = new QAction(tr("&New"), this);
    pMenuItem->setShortcuts(QKeySequence::New);
    pMenuItem->setStatusTip(tr("Create a new file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::fileNew);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("&Add Experiment"), this);
    pMenuItem->setShortcuts(QKeySequence::New);
    pMenuItem->setStatusTip(tr("Add an experiment to the list"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::fileAddExperiment);
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
    connect(mpExpLoad, &QAction::triggered, this, &cMainWindow::experimentLoad);
    mpExperimentMenu->addAction(mpExpLoad);

    mpExperimentMenu->addSeparator();

    mpExpRun = new QAction(tr("&Run"), this);
    mpExpRun->setStatusTip(tr("Run experiment..."));
    connect(mpExpRun, &QAction::triggered, this, &cMainWindow::experimentRun);
    mpExperimentMenu->addAction(mpExpRun);

    mpExpPause = new QAction(tr("&Pause"), this);
    mpExpPause->setStatusTip(tr("Pause the currently running experiment"));
    connect(mpExpPause, &QAction::triggered, this, &cMainWindow::experimentPause);
    mpExperimentMenu->addAction(mpExpPause);
    mpExpPause->setEnabled(false);

    mpExpStop = new QAction(tr("&Stop"), this);
    mpExpStop->setStatusTip(tr("Stop the currently running experiment"));
    connect(mpExpStop, &QAction::triggered, this, &cMainWindow::experimentStop);
    mpExperimentMenu->addAction(mpExpStop);
    mpExpStop->setEnabled(false);

    // Build the View Menu
    /* The view menu is built dock window system */

    // Build the Help Menu
    pMenuItem = new QAction(tr("&About"), this);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::helpAbout);
    mpHelpMenu->addAction(pMenuItem);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createActions()
{

}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createToolBars()
{
//    mpFileBar = addToolBar("File");

    auto* toolbar = new cExperimentToolbar(this);
    addToolBar(toolbar);
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createStatusBar()
{
    statusBar();
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createDockWindows()
{
    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new cExperimentManager(dock);

    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
bool cRemoteClientWindow::createExperimentController()
{
    std::string cfgFileName = getCfgFilePath();
    if (cfgFileName.empty())
    {
        return false;
    }

    QDockWidget* dockWidget = nullptr;

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        QString msg = "Could not open ";
        msg += cfgFileName.c_str();
        msg += " for reading!";

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
        mb.exec();

        return false;
    }

    try
    {
        nlohmann::json jsonDoc;
        in >> jsonDoc;

        if (!jsonDoc.contains("controller"))
        {
            std::string msg = "Fatal error in ";
            msg += cfgFileName;
            msg += ": Undefined experiment controller";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
            mb.exec();

            return false;
        }

        std::string name = jsonDoc["controller"];

        auto widgets = create_experiment_controller(name);

        cExperimentControlModel* pModel = widgets.pModel;
        mpController = widgets.pView;

        if ((pModel == nullptr) || (mpController == nullptr))
        {
            //continue;
        }

        QObject::connect(pModel, &cExperimentControlModel::statusMessage, this, &cMainWindow::onStatusUpdate);
        QObject::connect(pModel, &cExperimentControlModel::infoMessage, this, &cMainWindow::onInfoMessage);
        QObject::connect(pModel, &cExperimentControlModel::warningMessage, this, &cMainWindow::onWarningMessage);
        QObject::connect(pModel, &cExperimentControlModel::errorMessage, this, &cMainWindow::onErrorMessage);

        mMainModel.addExperimentControlModel(pModel);

        if (jsonDoc.contains(name))
        {
            pModel->configure(jsonDoc[name]);
            mpController->configure(jsonDoc[name]);
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
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error in ";
        msg += cfgFileName;
        msg += ": ";
        msg += e.what();

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
        mb.exec();

        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void cRemoteClientWindow::createSensorModelsAndViews()
{
    std::string cfgFileName = getCfgFilePath();
    if (cfgFileName.empty())
    {
        return;
    }

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        std::string msg = "Could not open ";
        msg += cfgFileName;
        msg += " for reading!";

        throw std::runtime_error(msg);
    }

    try
    {
        nlohmann::json jsonDoc;
        in >> jsonDoc;

        auto sensors = jsonDoc["sensors"];

        for (auto sensor : sensors)
        {
            std::string type = sensor["type"];
            auto widgets = create_sensor(type, sensor);

            if (widgets.pModel == nullptr)
            {
                std::string msg = "Error in ";
                msg += cfgFileName;
                msg += ": Unknown sensor type \"";
                msg += type;
                msg += "\".";

                QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
                mb.exec();
                continue;
            }

            QObject::connect(widgets.pModel, &cSensorModel::statusMessage, this, &cMainWindow::onStatusUpdate);
            QObject::connect(widgets.pModel, &cSensorModel::infoMessage, this, &cMainWindow::onInfoMessage);
            QObject::connect(widgets.pModel, &cSensorModel::warningMessage, this, &cMainWindow::onWarningMessage);
            QObject::connect(widgets.pModel, &cSensorModel::errorMessage, this, &cMainWindow::onErrorMessage);

            if (jsonDoc.contains(type))
            {
                bool validSensor = false;
                try
                {
                    validSensor = widgets.pModel->configure(jsonDoc[type]);
                }
                catch (const std::exception& e)
                {
                    validSensor = false;
                }

/*BAF
                if (!validSensor)
                {
                    remove_sensor(type, widgets);
                    continue;
                }
*/
            }


            mMainModel.addSensor(widgets.pModel);

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

        throw std::runtime_error(msg);
    }
}
