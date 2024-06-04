
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "CreateExperimentFromGpsDlg.hpp"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"
#include "FieldLayoutWidget.hpp"
#include "ExperimentDesignWidget.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "ExperimentFieldLayoutDlg.hpp"

#include "RappFieldBoundary.hpp"
#include "FieldBoundaryDlg.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QSound>
#include <QMdiArea>

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

    connect(mpExpDesign, &cExperimentDesignWidget::insertBefore, this, &cMainWindow::onInsertStepBefore);
    connect(mpExpDesign, &cExperimentDesignWidget::insertAfter, this, &cMainWindow::onInsertStepAfter);
    connect(mpExpDesign, &cExperimentDesignWidget::deleteStep, this, &cMainWindow::onDeleteStep);

    setCentralWidget(mpExpDesign);

//    QMdiArea* mdiArea = new QMdiArea(this);
//    setCentralWidget(mdiArea);
}

//-----------------------------------------------------------------------------
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

    mpEditMenu->setDisabled(true);
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
    pMenuItem = new QAction(tr("Edit Experiment Meta Data..."), this);
    pMenuItem->setStatusTip(tr("Edit the experiment meta information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditExperimentMetaInfo);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Edit Experiment Controller Information..."), this);
    pMenuItem->setStatusTip(tr("Edit the experiment controller information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditExperimentCtrlInfo);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Edit Experiment Sensor Information..."), this);
    pMenuItem->setStatusTip(tr("Edit the experiment sensor information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditExperimentSernsorInfo);
    mpEditMenu->addAction(pMenuItem);

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Add Experiment To Layout..."), this);
    pMenuItem->setStatusTip(tr("Adds the experiment to the field layout..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditAddExperimentToLayout);
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
    connect(mpExperiments, &cExperimentManager::loadExperiment, this, &cMainWindow::onOpenExperiment);

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

    if (mpFieldLayout->isDirty())
    {
        mpFieldLayout->save(mFieldLayoutFile);
    }
}


/********************************************************************
 * Slots associated with "File" menu actions
 *******************************************************************/
void cMainWindow::onFileNewExperiment_Blank()
{
    doSaveCheck();

    if (!mExperimentFile.getController())
    {
        std::unique_ptr<cExperimentCtrlInfo> ctrl = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
        mExperimentFile.setController(std::move(ctrl));
    }

    mExperimentFile.clearSteps();

    mpExpDesign->loadExperiment(mExperimentFile);

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onFileNewExperiment_GPS()
{
    doSaveCheck();

    QString defaultDirectory = mSettings.value("Defaults/gpsFiles").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open GPS File"), defaultDirectory,
        "GPS Files (*.csv)");

    if (fileName.isEmpty())
        return;

    std::ifstream gps_file;
    gps_file.open(fileName.toStdString());

    if (!gps_file.is_open())
    {
        QString msg = "Could not open file: ";
        msg += fileName;
        QMessageBox msg_box(QMessageBox::Critical, "File Error", msg);
        msg_box.exec();
        return;
    }

    std::string test;
    gps_file >> test;
    gps_file.close();

    if (test != "ILUC,1249989.825,1015874.374,872.219,ILUC")
    {
        QString msg = "Invalid GPS file: ";
        msg += fileName;
        QMessageBox msg_box(QMessageBox::Critical, "Invalid File", msg);
        msg_box.exec();
        return;
    }

    std::filesystem::path file_name = fileName.toStdString();

    std::filesystem::path directory = file_name.parent_path();

    mSettings.setValue("Defaults/gpsFiles", QString::fromStdString(directory.string()));

    cCreateExperimentFromGpsDlg dlg(fileName, this);

    connect(&dlg, &cCreateExperimentFromGpsDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateExperimentFromGpsDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateExperimentFromGpsDlg::experimentChanged, this, &cMainWindow::onExperimentChange);
    connect(&dlg, &cCreateExperimentFromGpsDlg::saveExperiment, this, &cMainWindow::onFileSaveAsExperimentFile);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        mExperimentFile.clear();
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onFileOpenExperiment()
{
    QString defaultDirectory = mSettings.value("Defaults/experimentDirectory").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Experiment File"), defaultDirectory,
        "Experiment Files (*.json)");

    onOpenExperiment(fileName);
}

void cMainWindow::onFileSaveExperimentFile()
{
    if (mExperimentFile.isDirty())
    {
        if (mExperimentFile.getFileName().empty())
            onFileSaveAsExperimentFile();
        else
            mExperimentFile.save();
    }
}

void cMainWindow::onFileSaveAsExperimentFile()
{
    QString defaultDirectory = mSettings.value("Defaults/experimentDirectory").toString();

    if (!mExperimentFile.getFileName().empty())
        defaultDirectory = QString::fromStdString(mExperimentFile.getFileName());

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Experiment File"), defaultDirectory,
        "Experiment Files (*.json)");

    if (fileName.isEmpty())
        return;

    mExperimentFile.save_as(fileName.toStdString());

    QString title = "RAPP Plot Mapper - ";
    title += fileName;
    setWindowTitle(title);

    mpExperiments->reloadExperiments();
}


/********************************************************************
 * Slots associated with "Edit" menu actions
 *******************************************************************/
void cMainWindow::onEditExperimentMetaInfo()
{
    cExperimentMetaInfoDlg dlg(mExperimentFile.getMetaData(), this);

    dlg.setExperimentTitle(mExperimentFile.getExperimentName());

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    mExperimentFile.setExperimentName(dlg.getExperimentTitle());
}

void cMainWindow::onEditExperimentCtrlInfo()
{
    cExperimentCtrlInfoDlg dlg(mExperimentFile, this);
    dlg.exec();
}

void cMainWindow::onEditExperimentSernsorInfo()
{
//    cExperimentSensorInfoDlg dlg(mExperimentFile, this);
//    dlg.exec();
}

void cMainWindow::onEditAddExperimentToLayout()
{
    cExperimentFieldLayoutDlg dlg(*mpFieldLayout, this);

    dlg.setExperiment(mExperimentFile);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto original = dlg.getOriginalLayout();
    auto new_layout = dlg.getLayout();

    mpFieldLayout->replaceLayout(original, new_layout);

    mExperimentFile.setLayoutName(new_layout.caption.label.toStdString());
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
    QString layoutFile = QFileDialog::getOpenFileName(this, tr("Select the Field Layout File..."), mFieldLayoutFile,
        tr("Field Layout (*.json)"));

    if (layoutFile.isEmpty())
        return;

    mSettings.setValue("Defaults/fieldLayoutFile", layoutFile);
    mFieldLayoutFile = layoutFile;
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

/********************************************************************
 * General Purpose Slots
 *******************************************************************/
void cMainWindow::onOpenExperiment(const QString& filename)
{
    if (filename.isEmpty())
        return;

    doSaveCheck();

    mExperimentFile.clear();

    mExperimentFile.open(filename.toStdString());

    QString title = "RAPP Plot Mapper - ";
    title += filename;
    setWindowTitle(title);

    mpExpDesign->loadExperiment(mExperimentFile);

    mpEditMenu->setDisabled(false);
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentChange(QSharedPointer<cExperimentFile> experiment)
{
    mExperimentFile = *experiment;
    mpExpDesign->loadExperiment(mExperimentFile);
}

void cMainWindow::onInsertStepBefore(int id, int type)
{
    switch (type)
    {
    case eExperimentStep::delay:
    {
        auto step = std::make_unique<cExperimentStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    case eExperimentStep::pause:
    {
        auto step = std::make_unique<cExperimentStep_Pause>();
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    case eExperimentStep::movement:
    {
        auto step = std::make_unique<cExperimentStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertBefore(id, std::move(step));
        break;
    }
    default:
        return;
    }

    mpExpDesign->loadExperiment(mExperimentFile);
}

void cMainWindow::onInsertStepAfter(int id, int type)
{
    switch (type)
    {
    case eExperimentStep::delay:
    {
        auto step = std::make_unique<cExperimentStep_Delay>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    case eExperimentStep::pause:
    {
        auto step = std::make_unique<cExperimentStep_Pause>();
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    case eExperimentStep::movement:
    {
        auto step = std::make_unique<cExperimentStep_Movement>();
        if (!step->onEdit())
        {
            return;
        }
        mExperimentFile.insertAfter(id, std::move(step));
        break;
    }
    default:
        return;
    }

    mpExpDesign->loadExperiment(mExperimentFile);
}

void cMainWindow::onDeleteStep(int id)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete the experiment step?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        if (mExperimentFile.removeStep(id))
            mpExpDesign->loadExperiment(mExperimentFile);
    }
}

//-----------------------------------------------------------------------------
void cMainWindow::closeEvent(QCloseEvent* event)
{
    if (mExperimentFile.isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText("The experiment file has been modified.");
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
            event->ignore();
            return;
        }
    }

//    mSettings.setValue("mainWindow/geometry", saveGeometry());
//    mSettings.setValue("mainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
}

