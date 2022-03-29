
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "CeresSplashScreen.hpp"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"

#include "ExperimentCtrlFactory.hpp"
#include "ExperimentControllers/ExperimentCtrlView.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"

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
cMainWindow::cMainWindow(QWidget* parent) :
    QMainWindow(parent),
    mpSplashScreen(nullptr),
    mpExperiments(nullptr),
    mpFileMenu(nullptr),
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
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    mMainModel.stopDataThread();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize(cCeresSplashScreen* pSplashScreen)
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
void cMainWindow::fileNew()
{
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
    mMainModel.loadExperiment(expDoc);

    msg = "Running experiment: ";
    msg += pExperiment->text(0);

    onStatusUpdate(msg);

    std::string filename = "c:\tmp\test.data";
    mMainModel.startExperiment(filename);
}

//-----------------------------------------------------------------------------
void cMainWindow::fileAddExperiment()
{
    QTreeWidgetItem* experiment = new QTreeWidgetItem();
    experiment->setText(0, "Hello");
    QTreeWidgetItem* title = new QTreeWidgetItem();
    title->setText(0, "Title");
    experiment->addChild(title);
    mpExperiments->addTopLevelItem(experiment);
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

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
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

    // Build the View Menu

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
    mpFileBar = addToolBar("File");
}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows()
{
    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new cExperimentManager(dock);

    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
bool cMainWindow::createExperimentController()
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
void cMainWindow::createSensorModelsAndViews()
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
