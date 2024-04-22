
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"
#include "FieldLayoutWidget.hpp"
#include "ExperimentDesignWidget.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"

#include "RappFieldBoundary.hpp"
#include "FieldBoundaryDlg.hpp"

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

    auto cwd = std::filesystem::current_path();

    mExperimentFilesPath = mSettings.value("Defaults/experimentDirectory", cwd.c_str()).toString();
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

    mpExpDesign = new cExperimentDesignWidget(this);
    connect(mpExpDesign, &cExperimentDesignWidget::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(mpExpDesign, &cExperimentDesignWidget::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);

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

    QMenu* newMenu = mpFileMenu->addMenu(tr("New Experiment File..."));

    pMenuItem = new QAction(tr("Blank"), this);
    pMenuItem->setStatusTip(tr("Creates a blank experiment file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileNewExperiment_Blank);
    newMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("From GPS data"), this);
    pMenuItem->setStatusTip(tr("Creates an experiment file from GPS data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileNewExperiment_GPS);
    newMenu->addAction(pMenuItem);

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

    pMenuItem = new QAction(tr("Default Field Layout File"), this);
    pMenuItem->setStatusTip(tr("The default file for saving/loading the field layout"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultFieldLayoutFile);
    mpPreferencesMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Default Plot Split Directory"), this);
    pMenuItem->setStatusTip(tr("Sets the default directory for saving/loading plot split files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultPlotSplitDirectory);
    mpPreferencesMenu->addAction(pMenuItem);

    mpPreferencesMenu->addSeparator();

    pMenuItem = new QAction(tr("Default Field Boundaries"), this);
    pMenuItem->setStatusTip(tr("Sets the default field boundaries"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultFieldBoundaries);
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

    auto minX_mm = mSettings.value("Defaults/fieldBounds/minX_mm", 0).toInt();
    auto maxX_mm = mSettings.value("Defaults/fieldBounds/maxX_mm", 190000).toInt();
    auto minY_mm = mSettings.value("Defaults/fieldBounds/minY_mm", 0).toInt();
    auto maxY_mm = mSettings.value("Defaults/fieldBounds/maxY_mm", 190000).toInt();

    mpFieldLayout->setBounds(minX_mm, maxX_mm, minY_mm, maxY_mm);

    QString defaultFile = mSettings.value("Defaults/fieldLayoutFile").toString();
    mpFieldLayout->load(defaultFile);

    dock->setWidget(mpFieldLayout);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDataModel(const nlohmann::json& configDoc)
{
}

//-----------------------------------------------------------------------------
void cMainWindow::doSaveCheck()
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
}


/********************************************************************
 * Slots associated with "File" menu actions
 *******************************************************************/
void cMainWindow::onFileNewExperiment_Blank()
{
    doSaveCheck();

    mExperimentFile.clearSteps();

    mpExpDesign->loadExperiment(mExperimentFile);
}

void cMainWindow::onFileNewExperiment_GPS()
{
    doSaveCheck();

    mExperimentFile.clearSteps();
}

void cMainWindow::onFileOpenExperiment()
{
    doSaveCheck();

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

void cMainWindow::onPreferenceDefaultFieldBoundaries()
{
    auto minX_mm = mSettings.value("Defaults/fieldBounds/minX_mm", 0).toInt();
    auto maxX_mm = mSettings.value("Defaults/fieldBounds/maxX_mm", 190000).toInt();
    auto minY_mm = mSettings.value("Defaults/fieldBounds/minY_mm", 0).toInt();
    auto maxY_mm = mSettings.value("Defaults/fieldBounds/maxY_mm", 190000).toInt();

    cFieldBoundaryDlg dlg(this);

    dlg.setMinX_mm(minX_mm);
    dlg.setMaxX_mm(maxX_mm);
    dlg.setMinY_mm(minY_mm);
    dlg.setMaxY_mm(maxY_mm);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    minX_mm = dlg.minX_mm();
    maxX_mm = dlg.maxX_mm();
    minY_mm = dlg.minY_mm();
    maxY_mm = dlg.maxY_mm();

    mpFieldLayout->setBounds(minX_mm, maxX_mm, minY_mm, maxY_mm);

    mSettings.setValue("Defaults/fieldBounds/minX_mm", minX_mm);
    mSettings.setValue("Defaults/fieldBounds/maxX_mm", maxX_mm);
    mSettings.setValue("Defaults/fieldBounds/minY_mm", minY_mm);
    mSettings.setValue("Defaults/fieldBounds/maxY_mm", maxY_mm);
}

/********************************************************************
 * Slots associated with "Help" menu actions
 *******************************************************************/
void cMainWindow::onHelpAbout()
{
}

