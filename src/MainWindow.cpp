
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

    mMainModel.startDataThread();

    mpSplashScreen = nullptr;
    onStatusUpdate(tr("Ready"));

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
    mMainModel.startExperiment();
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
    QStatusBar* pStatus = statusBar();
    
    QFontMetrics fm(pStatus->font());
    int pixelsWide = fm.horizontalAdvance(" XXX.X ");

    mpWindSpeed_kts = new QLineEdit();
    mpWindSpeed_kts->setReadOnly(true);
    mpWindSpeed_kts->setFixedWidth(pixelsWide);
    mpWindSpeed_kts->setToolTip(tr("Wind Speed in knots"));

    myWindDirection_deg = new QLineEdit();
    myWindDirection_deg->setReadOnly(true);
    myWindDirection_deg->setFixedWidth(pixelsWide);
    myWindDirection_deg->setToolTip(tr("Wind Direction"));

    pStatus->addPermanentWidget(mpWindSpeed_kts);
    pStatus->addPermanentWidget(myWindDirection_deg);
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
            widgets.pDockableView->setParent(this);
            widgets.pDockableView->setAllowedAreas(Qt::AllDockWidgetAreas);
            widgets.pDockableView->hide();

            addDockWidget(Qt::BottomDockWidgetArea, widgets.pDockableView);
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
            auto widgets = create_sensor(name);

            if ((widgets.pModel == nullptr) || (widgets.pView == nullptr))
                continue;

            if (jsonDoc.contains(name))
            {
                bool validSensor = false;
                try
                {
                    validSensor = widgets.pModel->configure(jsonDoc[name]);
                }
                catch (const std::exception& e)
                {
                    validSensor = false;
                }

                if (!validSensor)
                {
                    remove_sensor(name, widgets);
                    continue;
                }
            }

            widgets.pView->setParent(this);

            QObject::connect(widgets.pModel, &cSensorModel::statusMessage, this, &cMainWindow::onStatusUpdate);

            mMainModel.addSensor(widgets.pModel);

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
