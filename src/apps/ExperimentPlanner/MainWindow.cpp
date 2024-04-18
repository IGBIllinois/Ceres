
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"
#include "FieldLayoutWidget.hpp"
#include "ExperimentDesignWidget.hpp"

#include "ExperimentMetaInfoDlg.hpp"

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
    mpUI(new Ui::MainWindow),
    mSettings("UIUC", "Ceres Experiment Planner")
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Experiment Planner"));

    setUnifiedTitleAndToolBarOnMac(true);

    mExperimentFilesPath = mSettings.value("Defaults/experimentDirectory").toString();
    mFieldLayoutFile = mSettings.value("Defaults/fieldLayoutFile").toString();
    mPlotSplitsPath = mSettings.value("Defaults/plotSplitDirectory").toString();
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
    createMainMenu();
    createSubMenusAndActions();
    createActions();

    createToolBars();
    createDockWindows();

    createStatusBar();

/*
    mpScanArea = new cSpidercamScanArea(this);

    auto* mainlayout = new QVBoxLayout();
    mainlayout->addSpacing(10);
    mainlayout->addWidget(mpScanArea);
    mainlayout->addSpacing(10);

    auto* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainlayout);

    setCentralWidget(centralWidget);
*/

    mpExpDesign = new cExperimentDesignWidget(this);
    setCentralWidget(mpExpDesign);
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpEditMenu = mpUI->menuBar->addMenu(tr("&Edit"));
    mpPreferencesMenu = mpUI->menuBar->addMenu(tr("&Preferences"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    //
    // Build the File Sub Menu
    //
    pMenuItem = new QAction(tr("New Experiment File"), this);
    pMenuItem->setStatusTip(tr("Create a new experiment file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileNewExperiment);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Open Experiment File..."), this);
    pMenuItem->setStatusTip(tr("Loads experiment file into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileOpenExperiment);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Save Experiment File"), this);
    pMenuItem->setStatusTip(tr("Save the experiment file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveExperimentFile);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Save Experiment File As..."), this);
    pMenuItem->setStatusTip(tr("Save the experiment file with a different file name"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveAsExperimentFile);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);

    //
    // Build the Edit Sub Menu
    //
    pMenuItem = new QAction(tr("&Edit Experiment Meta Data..."), this);
    pMenuItem->setStatusTip(tr("Edit the experiment meta information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditExperimentMetaInfo);
    mpEditMenu->addAction(pMenuItem);

    //
    // Build the Preference Sub Menu
    //
    pMenuItem = new QAction(tr("Default Experiment Directory"), this);
    pMenuItem->setStatusTip(tr("Sets the default directory for saving/loading experiment files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultExperimentDirectory);
    mpPreferencesMenu->addAction(pMenuItem);

    // Build the View Menu
    /* The view menu is built by the dock window system */

    // Build the Help Menu
    pMenuItem = new QAction(tr("&About"), this);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onHelpAbout);
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
void cMainWindow::createDockWindows()
{
    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new cExperimentManager(mExperimentFilesPath, dock);

    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Field Layout"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpFieldLayout = new cFieldLayoutWidget(dock);
    mpFieldLayout->initialize();

    dock->setWidget(mpFieldLayout);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDataModel(const nlohmann::json& configDoc)
{
}


/********************************************************************
 * Slots associated with "File" menu actions
 *******************************************************************/
void cMainWindow::onFileNewExperiment()
{

}

void cMainWindow::onFileOpenExperiment()
{
    if (mExperimentFile.isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText("The experiment configuration file has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            onFileSaveExperimentFile();
        }
        else if (ret == QMessageBox::Cancel)
        {
            return;
        }
    }

    QString defaultDirectory = mSettings.value("Defaults/experimentDirectory").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Experiment File"), defaultDirectory,
        "Experiment Files (*.json)");

    if (fileName.isEmpty())
        return;

    mExperimentFile.clear();

    mExperimentFile.open(fileName.toStdString());

    QString title = "RAPP Plot Mapper - ";
    title += fileName;
    setWindowTitle(title);

    mpExpDesign->loadExperiment(mExperimentFile);
}

void cMainWindow::onFileSaveExperimentFile()
{

}

void cMainWindow::onFileSaveAsExperimentFile()
{

}


/********************************************************************
 * Slots associated with "Edit" menu actions
 *******************************************************************/
void cMainWindow::onEditExperimentMetaInfo()
{
    cExperimentMetaInfoDlg dlg(this);

    dlg.exec();
}

/********************************************************************
 * Slots associated with "Preference" menu actions
 *******************************************************************/
void cMainWindow::onPreferenceDefaultExperimentDirectory()
{
    QString defaultDirectory = mSettings.value("Defaults/experimentDirectory").toString();

    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Default Directory for Saving/Loading Experiment Files..."), defaultDirectory);

    if (directory.isEmpty())
        return;

    mSettings.setValue("Defaults/experimentDirectory", directory);
}

void cMainWindow::onPreferenceDefaultFieldLayoutFile()
{
    QString defaultFile = mSettings.value("Defaults/fieldLayoutFile").toString();

    QString layoutFile = QFileDialog::getOpenFileName(this, tr("Select the Field Layout File..."), defaultFile,
        tr("Field Layout (*.json)"));

    if (layoutFile.isEmpty())
        return;

    mSettings.setValue("Defaults/fieldLayoutFile", layoutFile);
}

void cMainWindow::onPreferenceDefaultPlotSplitDirectory()
{
    QString defaultDirectory = mSettings.value("Defaults/plotSplitDirectory").toString();

    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Default Directory for Saving/Loading Plot Split Files..."), defaultDirectory);

    if (directory.isEmpty())
        return;

    mSettings.setValue("Defaults/plotSplitDirectory", directory);
}

/********************************************************************
 * Slots associated with "Help" menu actions
 *******************************************************************/
void cMainWindow::onHelpAbout()
{
}

