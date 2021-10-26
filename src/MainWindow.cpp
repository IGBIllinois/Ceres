
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

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
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    mMainModel.stopDataThread();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize(QSplashScreen* pSplashScreen)
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

    mMainModel.startDataThread();

    mpSplashScreen = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::fileNew()
{

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
        mpSplashScreen->showMessage(msg, Qt::AlignHCenter | Qt::AlignBottom);
        return;
    }

    if (statusBar())
        statusBar()->showMessage(msg);
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
    statusBar()->showMessage(tr("Ready"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows()
{
    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new QTreeWidget(dock);
    mpExperiments->clear();
    mpExperiments->setColumnCount(1);
    mpExperiments->setHeaderLabel("Loaded Experiments");

    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
bool cMainWindow::createExperimentController()
{
    std::string cfgFileName = "C:/igb/Ceres/build/bin/Debug/ceres.json";
    //    QApplication::arguments();
    QString s = QApplication::applicationDirPath();
    QDockWidget* dockWidget = nullptr;

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        // Note: we can't reuse the wxThreadEvent object
        std::string msg = "Could not open ";
        msg += cfgFileName;
        msg += " for reading!";

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
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

        mMainModel.addExperimentControlModel(pModel);

        if (jsonDoc.contains(name))
        {
            pModel->configure(jsonDoc[name]);
            mpController->configure(jsonDoc[name]);
        }

        setCentralWidget(mpController);

        if (widgets.pDockableView)
        {
            addDockWidget(Qt::NoDockWidgetArea, widgets.pDockableView);
            mpViewMenu->addAction(widgets.pDockableView->toggleViewAction());

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
    std::string cfgFileName = "C:/igb/Ceres/build/bin/Debug/ceres.json";
//    QApplication::arguments();
    QString s = QApplication::applicationDirPath();
    QDockWidget* dockWidget = nullptr;

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        // Note: we can't reuse the wxThreadEvent object
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

        for (std::string name : sensors)
        {
            auto widgets = create_sensor(name, this);

            if ((widgets.pModel == nullptr) || (widgets.pView == nullptr))
                continue;

            QObject::connect(widgets.pModel, &cSensorModel::statusMessage, this, &cMainWindow::onStatusUpdate);

            mMainModel.addSensor(widgets.pModel);

            if (jsonDoc.contains(name))
            {
                widgets.pModel->configure(jsonDoc[name]);
            }

            addDockWidget(Qt::RightDockWidgetArea, widgets.pView);
            mpViewMenu->addAction(widgets.pView->toggleViewAction());
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
