
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"

#include "RappFieldBoundary.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QSound>


#include <cassert>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>


//-----------------------------------------------------------------------------
cMainWindow::cMainWindow(QWidget* parent) :
    QMainWindow(parent),
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
    mpUI(new Ui::MainWindow)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Experiment Planner"));

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
    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize()
{
    onStatusUpdate("Initializing menus...");
    createMainMenu();
    createSubMenusAndActions();
    createActions();

    onStatusUpdate("Initializing toolbars...");
    createToolBars();

    onStatusUpdate("Initializing status bar...");
    createStatusBar();
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
    auto* pExperiment = static_cast<cExperimentTreeItem*>(mpExperiments->currentItem());

    if (pExperiment == nullptr)
    {
        return;
    }

    if (pExperiment->hasExperimentDocument())
    {
        loadExperiment(*pExperiment);
        return;
    }

    if (0 == pExperiment->childCount())
    {
        return;
    }
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

    return true;
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentRun()
{
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentPause()
{
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentStop()
{
}

//-----------------------------------------------------------------------------
void cMainWindow::helpAbout()
{

}

void cMainWindow::onStatusUpdate(QString msg)
{
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
    onStatusUpdate(msg);
}

void cMainWindow::onExperimentTerminated()
{
    mpExpLoad->setEnabled(true);
    mpExpRun->setEnabled(true);
    mpExpPause->setEnabled(false);
    mpExpStop->setEnabled(false);

    emit experimentStopped();

    onStatusUpdate("Experiment stopped!");
}

void cMainWindow::onExperimentCompleted()
{
    mpExpLoad->setEnabled(true);
    mpExpRun->setEnabled(true);
    mpExpPause->setEnabled(false);
    mpExpStop->setEnabled(false);

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

//    addToolBar(toolbar);
}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();
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
}

void cMainWindow::addSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->addAction(pAction);
}

void cMainWindow::removeSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->removeAction(pAction);
}
