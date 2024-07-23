
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Constants.hpp"

#include "CreateLidarExperimentFromSpiderCamPointDlg.hpp"
#include "CreateLidarExperimentFromGpsDlg.hpp"
#include "CreateLidarExperimentFromPlotInfoDlg.hpp"

#include "CreateHyperspectralReferenceExperimentFromSpiderCamPointDlg.hpp"
#include "CreateHyperspectralReferenceExperimentFromGpsDlg.hpp"

#include "CreateHyperspectralExperimentFromSpiderCamPointDlg.hpp"
#include "CreateHyperspectralExperimentFromGpsDlg.hpp"
#include "CreateHyperspectralExperimentFromPlotInfoDlg.hpp"

#include "ExperimentManager.hpp"
#include "ExperimentTreeItem.hpp"
#include "FieldLayoutWidget.hpp"
#include "ExperimentDesignWidget.hpp"
#include "ExperimentDesignMdiChild.hpp"

#include "ExperimentShiftDlg.hpp"
#include "NewSpidercamPositionDlg.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "ExperimentFieldLayoutDlg.hpp"

#include "RappFieldBoundary.hpp"
#include "FieldBoundaryDlg.hpp"
#include "GpsFileReader.hpp"
#include "FieldUtils.hpp"

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

    QString fileName = mSettings.value("Defaults/groundMeshFile").toString();

    if (!fileName.isEmpty())
        LoadGpsData(fileName);
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

    mpMdiArea = new QMdiArea(this);
    mpMdiArea->setViewMode(QMdiArea::TabbedView);
    mpMdiArea->setTabsClosable(true);
    setCentralWidget(mpMdiArea);
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
    mpGenerateMenu = mpUI->menuBar->addMenu(tr("&Generate"));
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

    pMenuItem = new QAction(tr("New Experiment File"), this);
    pMenuItem->setStatusTip(tr("Creates a blank experiment file"));
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

    pMenuItem = new QAction(tr("Save All Experiment Files"), this);
    pMenuItem->setStatusTip(tr("Save all experiment files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveAllExperimentFiles);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Close Experiment File"), this);
    pMenuItem->setStatusTip(tr("Close the experiment file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileCloseExperimentFile);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Close All Experiment Files"), this);
    pMenuItem->setStatusTip(tr("Close all the experiment files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileCloseAllExperimentFiles);
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

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Move the Experiment to a X-Position (north/south)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the x-position (north/south) within an experiment..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveExperimentX);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Move the Experiment to a Y-Position (east/west)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the y-position (east/west) within an experiment..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveExperimentY);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Move the Experiment to a Z-Position (vertical)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the z-position (vertical) within an experiment..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveExperimentZ);
    mpEditMenu->addAction(pMenuItem);

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Shift Experiment Positions..."), this);
    pMenuItem->setStatusTip(tr("Shift the positions used in the experiment..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditShiftExperiment);
    mpEditMenu->addAction(pMenuItem);


    //
    // Build the Generate Sub Menu
    //
    pMenuItem = new QAction(tr("LiDAR Scans From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan experiment file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("LiDAR Scans From GPS data (Machine Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan experiment file(s) from GPS (begin/end) data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("LiDAR Scans From GPS plot data (Hand Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan experiment file(s) from GPS plot data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_PlotInfo);
    mpGenerateMenu->addAction(pMenuItem);

    mpGenerateMenu->addSeparator();

    pMenuItem = new QAction(tr("Hyperspectral Reference Scan From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral reference experiment file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralRefScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Reference Scan From GPS Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral reference experiment file(s) from single GPS point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralRefScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    mpGenerateMenu->addSeparator();

    pMenuItem = new QAction(tr("Hyperspectral Scan From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral experiment file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Scan From GPS Points (Machine Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral experiment file(s) from GPS (begin/end) point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Scan From GPS plot data (Hand Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral experiment file(s) from GPS plot point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_PlotInfo);
    mpGenerateMenu->addAction(pMenuItem);

    //
    // Build the Preference Sub Menu
    //
    pMenuItem = new QAction(tr("Default Experiment Directory"), this);
    pMenuItem->setStatusTip(tr("Sets the default directory for saving/loading experiment files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultExperimentDirectory);
    mpPreferencesMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Load Ground Data"), this);
    pMenuItem->setStatusTip(tr("Load the GPS based ground data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceLoadGroundMesh);
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
cExperimentDesignMdiChild* cMainWindow::createMdiChild()
{
    cExperimentDesignMdiChild* child = new cExperimentDesignMdiChild(this);
    child->onDefaultExperimentPathChange(mExperimentFilesPath);

    mpMdiArea->addSubWindow(child);

    connect(child, &cExperimentDesignWidget::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(child, &cExperimentDesignWidget::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);

    connect(child, &cExperimentDesignMdiChild::experimentListNeedsUpdate, this, &cMainWindow::onExperimentListUpdateNeeded);

    connect(this, &cMainWindow::defaultExperimentPathChange, child, &cExperimentDesignMdiChild::onDefaultExperimentPathChange);

    return child;
}

//-----------------------------------------------------------------------------


/********************************************************************
 * Slots associated with "File" menu actions
 *******************************************************************/
void cMainWindow::onFileNewExperiment()
{
    auto* child = createMdiChild();
    child->newFile();
    child->show();

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onFileOpenExperiment()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Experiment File"), mExperimentFilesPath,
        "Experiment Files (*.json)");

    if (fileName.isEmpty())
        return;

    onOpenExperiment(fileName);
}

void cMainWindow::onFileSaveExperimentFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->save();
}

void cMainWindow::onFileSaveAsExperimentFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    child->saveAs();
}

void cMainWindow::onFileSaveAllExperimentFiles()
{
    auto list = mpMdiArea->subWindowList();
    for (auto* subWindow : list)
    {
        auto* child = static_cast<cExperimentDesignMdiChild*>(subWindow->widget());
        child->save();
    }
}

void cMainWindow::onFileCloseExperimentFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    childSubWindow->close();
}

void cMainWindow::onFileCloseAllExperimentFiles()
{
    mpMdiArea->closeAllSubWindows();
}


/********************************************************************
 * Slots associated with "Edit" menu actions
 *******************************************************************/
void cMainWindow::onEditExperimentMetaInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editMetaInfo();
}

void cMainWindow::onEditExperimentCtrlInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editCtrlInfo();
}

void cMainWindow::onEditExperimentSernsorInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editSensorInfo();
}

void cMainWindow::onEditAddExperimentToLayout()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    
    cExperimentFieldLayoutDlg dlg(*mpFieldLayout, this);

    dlg.setExperiment(child->getExperimentFile());

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto original = dlg.getOriginalLayout();
    auto new_layout = dlg.getLayout();

    mpFieldLayout->replaceLayout(original, new_layout);

    child->setLayoutName(new_layout.caption.label.toStdString());
}

void cMainWindow::onEditMoveExperimentX()
{
    cNewSpidercam_X_PositionDlg dlg;

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int x_mm = dlg.x_mm();

    child->set_X_Position(x_mm);

    child->reloadPath();
}

void cMainWindow::onEditMoveExperimentY()
{
    cNewSpidercam_Y_PositionDlg dlg;

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int y_mm = dlg.y_mm();

    child->set_Y_Position(y_mm);

    child->reloadPath();
}

void cMainWindow::onEditMoveExperimentZ()
{
    cNewSpidercam_Z_PositionDlg dlg;

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int z_mm = dlg.z_mm();

    child->set_Z_Position(z_mm);

    child->reloadPath();
}

void cMainWindow::onEditShiftExperiment()
{
    cExperimentShiftDlg dlg;

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int x_mm = dlg.xShift_mm();
    int y_mm = dlg.yShift_mm();
    int z_mm = dlg.zShift_mm();

    child->shiftPositions(x_mm, y_mm, z_mm);

    child->reloadPath();
}



/********************************************************************
 * Slots associated with "Generate" menu actions
 *******************************************************************/
void cMainWindow::onGenerateLidarScan_SpiderCam_Point()
{
    cCreateLidarExperimentFromSpiderCamDlg dlg(this);

    connect(&dlg, &cCreateLidarExperimentFromSpiderCamDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromSpiderCamDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromSpiderCamDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onGenerateLidarScan_GPS()
{
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

    cCreateLidarExperimentFromGpsDlg dlg(fileName, this);

    connect(&dlg, &cCreateLidarExperimentFromGpsDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromGpsDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromGpsDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onGenerateLidarScan_PlotInfo()
{
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

    cCreateLidarExperimentFromPlotInfoDlg dlg(fileName, this);

    connect(&dlg, &cCreateLidarExperimentFromPlotInfoDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromPlotInfoDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateLidarExperimentFromPlotInfoDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onGenerateHyperspectralRefScan_SpiderCam_Point()
{
    cCreateHyperspectralReferenceExperimentFromSpiderCamDlg dlg(this);

    connect(&dlg, &cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onGenerateHyperspectralRefScan_GPS()
{

}

void cMainWindow::onGenerateHyperspectralScan_SpiderCam_Point()
{
//    cCreateHyperspectralExperimentFromSpiderCamPointDlg dlg;
}

void cMainWindow::onGenerateHyperspectralScan_GPS()
{
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

    cCreateHyperspectralExperimentFromGpsDlg dlg(fileName, this);

    connect(&dlg, &cCreateHyperspectralExperimentFromGpsDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromGpsDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromGpsDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onGenerateHyperspectralScan_PlotInfo()
{
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

    cCreateHyperspectralExperimentFromPlotInfoDlg dlg(fileName, this);

    connect(&dlg, &cCreateHyperspectralExperimentFromPlotInfoDlg::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromPlotInfoDlg::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromPlotInfoDlg::experimentChanged, this, &cMainWindow::onExperimentChange);

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
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
    mExperimentFilesPath = directory;

    emit defaultExperimentPathChange(mExperimentFilesPath);
}

//-----------------------------------------------------------------------------
void cMainWindow::onPreferenceLoadGroundMesh()
{
    QString savedFileName = mSettings.value("Defaults/groundMeshFile").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Ground Data..."), savedFileName,
        "GPS CSV Files (*.csv)");

    // Open file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    // Return on Cancel
    if (!file.exists())
        return;

    LoadGpsData(fileName);

    mSettings.setValue("Defaults/groundMeshFile", fileName);
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
    mPlotSplitsPath = directory;
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

    auto list = mpMdiArea->subWindowList();
    for (auto* subWindow : list)
    {
        auto* child = static_cast<cExperimentDesignMdiChild*>(subWindow->widget());
        if (child->currentFile() == filename)
        {
            mpMdiArea->setActiveSubWindow(subWindow);
            return;
        }
    }
    auto* child = createMdiChild();
    child->loadFile(filename);
    child->show();

    mpEditMenu->setDisabled(false);
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentChange(QSharedPointer<cExperimentFile> experiment)
{
    if (experiment.isNull()) return;

    auto filename = experiment->getFileName();
    auto title = experiment->getExperimentName();

    auto list = mpMdiArea->subWindowList();
    for (auto* subWindow : list)
    {
        auto* child = static_cast<cExperimentDesignMdiChild*>(subWindow->widget());
        if ((!title.empty() && (child->getExperimentTitle() == title))
            || (!filename.empty() && (child->getFileName() == filename)))
        {
            child->setExperimentFile(*experiment);
            mpMdiArea->setActiveSubWindow(subWindow);
            return;
        }
    }
    auto* child = createMdiChild();
    child->newFile(*experiment);
    child->show();

    mpEditMenu->setDisabled(false);
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentListUpdateNeeded()
{
    mpExperiments->reloadExperiments();
}

//-----------------------------------------------------------------------------
void cMainWindow::closeEvent(QCloseEvent* event)
{
    mpMdiArea->closeAllSubWindows();
    if (mpMdiArea->currentSubWindow()) 
    {
        event->ignore();
        return;
    }

    if (mpFieldLayout->isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText("The field layout file has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            mpFieldLayout->save(mFieldLayoutFile);
        }
        else if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    mSettings.setValue("mainWindow/geometry", saveGeometry());
    mSettings.setValue("mainWindow/windowState", saveState());

    QMainWindow::closeEvent(event);
}

//-----------------------------------------------------------------------------
void cMainWindow::LoadGpsData(QString fileName)
{
    // Open file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    // Return on Cancel
    if (!file.exists())
        return;

    cGpsFileReader gps;
    gps.loadFromFile(fileName.toStdString());

    auto points = gps.GetPoints();

    std::vector<rfm::rappPoint_t> rapp_points;

    for (const auto& point : points)
    {
        std::int32_t x_mm = point.x_m * nConstants::M_TO_MM;
        std::int32_t y_mm = point.y_m * nConstants::M_TO_MM;
        std::int32_t z_mm = point.z_m * nConstants::M_TO_MM;

        rapp_points.emplace_back(x_mm , y_mm , z_mm);
    }

    mData.addGroundPoints(rapp_points);

    auto data = mData.getGroundPoints();
    auto mesh = computeGroundMesh(data);

    mData.clearGroundMesh();
    mData.addMeshData(mesh);
}

