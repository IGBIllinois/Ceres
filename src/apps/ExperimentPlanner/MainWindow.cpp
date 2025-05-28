
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "PlannerDataModel.hpp"
#include "PlannerDataModelLocal.hpp"

#include "Constants.hpp"

#include "CreateLidarExperimentFromSpiderCamPointDlg.hpp"
#include "CreateLidarExperimentFromGpsDlg.hpp"
#include "CreateLidarExperimentFromPlotInfoDlg.hpp"

#include "CreateHyperspectralReferenceExperimentFromSpiderCamPointDlg.hpp"
#include "CreateHyperspectralReferenceExperimentFromGpsDlg.hpp"

#include "CreateHyperspectralExperimentFromSpiderCamPointDlg.hpp"
#include "CreateHyperspectralExperimentFromGpsDlg.hpp"
#include "CreateHyperspectralExperimentFromPlotInfoDlg.hpp"

#include "MeasurementManager.hpp"
#include "MeasurementTreeItem.hpp"
#include "FieldLayoutWidget.hpp"
#include "ExperimentDesignWidget.hpp"
#include "ExperimentDesignMdiChild.hpp"

#include "ExperimentShiftDlg.hpp"
#include "NewSpidercamPositionDlg.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "ExperimentCtrlFactory.hpp"
#include "ExperimentCtrlModel.hpp"
#include "Spidercam/SpidercamModel.hpp"

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
    mpMeasurements(nullptr),
    mpUI(new Ui::MainWindow),
    mSettings("UIUC", "Ceres Measurement Planner")
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Measurement Planner"));

    setUnifiedTitleAndToolBarOnMac(true);

    auto cwd = std::filesystem::current_path();

    mMeasurementFilesPath = mSettings.value("Defaults/measurementDirectory", cwd.c_str()).toString();
    mFieldLayoutFile = mSettings.value("Defaults/fieldLayoutFile").toString();
    mPlotSplitsPath = mSettings.value("Defaults/plotSplitDirectory").toString();

    QString fileName = mSettings.value("Defaults/groundMeshFile").toString();

    if (!fileName.isEmpty())
        LoadGpsData(fileName);

    mLimits.minX_mm = 10'000;
    mLimits.maxX_mm = 190'000;
    mLimits.minY_mm = 10'000;
    mLimits.maxY_mm = 190'000;
    mLimits.minHeight_mm = 1'000;
    mLimits.maxHeight_mm = 9'000;
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    if (mpModel)
    {
        mpModel->stopDataThread();
    }

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize()
{
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

    try
    {
        if (configDoc.contains("spidercam"))
        {
            auto jsonCfg = configDoc["spidercam"];
            mLimits.minX_mm = static_cast<uint32_t>(jsonCfg["min X position (m)"] * nConstants::M_TO_MM);
            mLimits.maxX_mm = static_cast<uint32_t>(jsonCfg["max X position (m)"] * nConstants::M_TO_MM);
            mLimits.minY_mm = static_cast<uint32_t>(jsonCfg["min Y position (m)"] * nConstants::M_TO_MM);
            mLimits.maxY_mm = static_cast<uint32_t>(jsonCfg["max Y position (m)"] * nConstants::M_TO_MM);
            mLimits.minHeight_mm = static_cast<uint32_t>(jsonCfg["min height (m)"] * nConstants::M_TO_MM);
            mLimits.maxHeight_mm = static_cast<uint32_t>(jsonCfg["max height (m)"] * nConstants::M_TO_MM);
            auto maxSpeed_mmps = static_cast<uint32_t>(jsonCfg["max speed (m/s)"] * nConstants::M_TO_MM);
        }
    }
    catch (const std::exception& e)
    {
    }

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

void cMainWindow::onMeasurementTerminated()
{
    emit measurementCompleted();
}

void cMainWindow::onMeasurementCompleted()
{
    emit measurementCompleted();
}


//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpEditMenu = mpUI->menuBar->addMenu(tr("&Edit"));
    mpGenerateMenu = mpUI->menuBar->addMenu(tr("&Generate"));
    mpPreferencesMenu = mpUI->menuBar->addMenu(tr("&Preferences"));
    mpSpidercamMenu = mpUI->menuBar->addMenu(tr("&Spidercam"));
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

    pMenuItem = new QAction(tr("New Measurement File"), this);
    pMenuItem->setStatusTip(tr("Creates a blank measurement file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileNewMeasurement);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Open Measurement File..."), this);
    pMenuItem->setStatusTip(tr("Loads measurement file into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileOpenMeasurement);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Save Measurement File"), this);
    pMenuItem->setStatusTip(tr("Save the measurement file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveMeasurementFile);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Save Measurement File As..."), this);
    pMenuItem->setStatusTip(tr("Save the measurement file with a different file name"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveAsMeasurementFile);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Save All Measurement Files"), this);
    pMenuItem->setStatusTip(tr("Save all measurement files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveAllMeasurementFiles);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Close Measurement File"), this);
    pMenuItem->setStatusTip(tr("Close the measurement file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileCloseMeasurementFile);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Close All Measurement Files"), this);
    pMenuItem->setStatusTip(tr("Close all the measurement files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileCloseAllMeasurementFiles);
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
    pMenuItem = new QAction(tr("Edit Measurement Meta Data..."), this);
    pMenuItem->setStatusTip(tr("Edit the measurement meta information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMeasurementMetaInfo);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Edit Measurement Controller Information..."), this);
    pMenuItem->setStatusTip(tr("Edit the measurement controller information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMeasurementCtrlInfo);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Edit Measurement Sensor Information..."), this);
    pMenuItem->setStatusTip(tr("Edit the measurement sensor information..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMeasurementSernsorInfo);
    mpEditMenu->addAction(pMenuItem);

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Add Measurement To Layout..."), this);
    pMenuItem->setStatusTip(tr("Adds the measurement to the field layout..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditAddMeasurementToLayout);
    mpEditMenu->addAction(pMenuItem);

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Move the Measurement to a X-Position (north/south)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the x-position (north/south) within an measurement..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveMeasurementX);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Move the Measurement to a Y-Position (east/west)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the y-position (east/west) within an measurement..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveMeasurementY);
    mpEditMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Move the Measurement to a Z-Position (vertical)..."), this);
    pMenuItem->setStatusTip(tr("Move (over write) the z-position (vertical) within an measurement..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditMoveMeasurementZ);
    mpEditMenu->addAction(pMenuItem);

    mpEditMenu->addSeparator();

    pMenuItem = new QAction(tr("Shift Measurement Positions..."), this);
    pMenuItem->setStatusTip(tr("Shift the positions used in the measurement..."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onEditShiftMeasurement);
    mpEditMenu->addAction(pMenuItem);


    //
    // Build the Generate Sub Menu
    //
    pMenuItem = new QAction(tr("LiDAR Scans From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan measurement file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("LiDAR Scans From GPS data (Machine Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan measurement file(s) from GPS (begin/end) data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("LiDAR Scans From GPS plot data (Hand Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates LiDAR scan measurement file(s) from GPS plot data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateLidarScan_PlotInfo);
    mpGenerateMenu->addAction(pMenuItem);

    mpGenerateMenu->addSeparator();

    pMenuItem = new QAction(tr("Hyperspectral Reference Scan From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral reference measurement file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralRefScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Reference Scan From GPS Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral reference measurement file(s) from single GPS point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralRefScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    mpGenerateMenu->addSeparator();

    pMenuItem = new QAction(tr("Hyperspectral Scan From SpiderCam Point"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral measurement file(s) from single SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_SpiderCam_Point);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Scan From GPS Points (Machine Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral measurement file(s) from GPS (begin/end) point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_GPS);
    mpGenerateMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Hyperspectral Scan From GPS plot data (Hand Planted)"), this);
    pMenuItem->setStatusTip(tr("Creates hyperspectral measurement file(s) from GPS plot point"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onGenerateHyperspectralScan_PlotInfo);
    mpGenerateMenu->addAction(pMenuItem);

    //
    // Build the Preference Sub Menu
    //
    pMenuItem = new QAction(tr("Default Measurement Directory"), this);
    pMenuItem->setStatusTip(tr("Sets the default directory for saving/loading measurement files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onPreferenceDefaultMeasurementDirectory);
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

    //
    // Build the Preference Sub Menu
    //
    mpSpidercamConnect = new QAction(tr("Connect"), this);
    mpSpidercamConnect->setStatusTip(tr("Connect to Spidercam"));
    connect(mpSpidercamConnect, &QAction::triggered, this, &cMainWindow::onConnectToSpidercam);
    mpSpidercamMenu->addAction(mpSpidercamConnect);

    mpSpidercamMenu->addSeparator();

    mpTestMeasurement = new QAction(tr("Test Measurement"), this);
    mpTestMeasurement->setStatusTip(tr("Run the measurement in test mode (no data recording)"));
    mpTestMeasurement->setEnabled(false);
    connect(mpTestMeasurement, &QAction::triggered, this, &cMainWindow::onSpidercamTestMeasurement);
    mpSpidercamMenu->addAction(mpTestMeasurement);

    mpStopMeasurement = new QAction(tr("Stop Measurement"), this);
    mpStopMeasurement->setStatusTip(tr("Stop the running measurement"));
    mpStopMeasurement->setEnabled(false);
    connect(mpStopMeasurement, &QAction::triggered, this, &cMainWindow::onSpidercamStopMeasurement);
    mpSpidercamMenu->addAction(mpStopMeasurement);

    mpPauseRunMeasurement = new QAction(tr("Pause Measurement"), this);
    mpPauseRunMeasurement->setStatusTip(tr("Pause the running measurement"));
    mpPauseRunMeasurement->setEnabled(false);
    connect(mpPauseRunMeasurement, &QAction::triggered, this, &cMainWindow::onSpidercamPauseRunMeasurement);
    mpSpidercamMenu->addAction(mpPauseRunMeasurement);


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
    QDockWidget* dock = new QDockWidget(tr("Measurements"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    mpMeasurements = new cMeasurementManager(mMeasurementFilesPath, dock);
    connect(mpMeasurements, &cMeasurementManager::loadMeasurement, this, &cMainWindow::onOpenMeasurement);
    connect(mpMeasurements, &cMeasurementManager::runMeasurement,  this, &cMainWindow::onMeasurementRun);

    connect(this, &cMainWindow::connectedToController,      mpMeasurements, &cMeasurementManager::onConnectToSpidercam);
    connect(this, &cMainWindow::disconnectedFromController, mpMeasurements, &cMeasurementManager::onDisconnectFromSpidercam);

    dock->setWidget(mpMeasurements);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Field Layout"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpFieldLayout = new cFieldLayoutWidget(dock);
    mpFieldLayout->initialize();

    connect(this, &cMainWindow::connectedToController, mpFieldLayout, &cFieldLayoutWidget::onConnectToSpidercam);
    connect(this, &cMainWindow::disconnectedFromController, mpFieldLayout, &cFieldLayoutWidget::onDisconnectFromSpidercam);
    connect(this, &cMainWindow::measurementRunning, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    
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
    child->onDefaultExperimentPathChange(mMeasurementFilesPath);

    mpMdiArea->addSubWindow(child);

    connect(child, &cExperimentDesignWidget::clearPaths, mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(child, &cExperimentDesignWidget::drawPath, mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);

    connect(child, &cExperimentDesignMdiChild::experimentListNeedsUpdate, this, &cMainWindow::onMeasurementListUpdateNeeded);

    connect(this, &cMainWindow::defaultMeasurementPathChange, child, &cExperimentDesignMdiChild::onDefaultExperimentPathChange);

    return child;
}

//-----------------------------------------------------------------------------


/********************************************************************
 * Slots associated with "File" menu actions
 *******************************************************************/
void cMainWindow::onFileNewMeasurement()
{
    auto* child = createMdiChild();
    child->newFile();
    child->show();

    mpEditMenu->setDisabled(false);
}

void cMainWindow::onFileOpenMeasurement()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Measurement File"), mMeasurementFilesPath,
        "Measurement Files (*.json)");

    if (fileName.isEmpty())
        return;

    onOpenMeasurement(fileName);
}

void cMainWindow::onFileSaveMeasurementFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->save();
}

void cMainWindow::onFileSaveAsMeasurementFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    child->saveAs();
}

void cMainWindow::onFileSaveAllMeasurementFiles()
{
    auto list = mpMdiArea->subWindowList();
    for (auto* subWindow : list)
    {
        auto* child = static_cast<cExperimentDesignMdiChild*>(subWindow->widget());
        child->save();
    }
}

void cMainWindow::onFileCloseMeasurementFile()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    childSubWindow->close();
}

void cMainWindow::onFileCloseAllMeasurementFiles()
{
    mpMdiArea->closeAllSubWindows();
}


/********************************************************************
 * Slots associated with "Edit" menu actions
 *******************************************************************/
void cMainWindow::onEditMeasurementMetaInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editMetaInfo();
}

void cMainWindow::onEditMeasurementCtrlInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editCtrlInfo();
}

void cMainWindow::onEditMeasurementSernsorInfo()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    child->editSensorInfo();
}

void cMainWindow::onEditAddMeasurementToLayout()
{
    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());
    
    cExperimentFieldLayoutDlg dlg(*mpFieldLayout, this);

    dlg.setExperiment(child->getMeasurementFile());

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto original = dlg.getOriginalLayout();
    auto new_layout = dlg.getLayout();

    mpFieldLayout->replaceLayout(original, new_layout);

    child->setLayoutName(new_layout.caption.label.toStdString());
}

void cMainWindow::onEditMoveMeasurementX()
{
    cNewSpidercam_X_PositionDlg dlg(mLimits.minX_mm, mLimits.maxX_mm);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cNewSpidercam_X_PositionDlg::positionUpdated);
    }

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

void cMainWindow::onEditMoveMeasurementY()
{
    cNewSpidercam_Y_PositionDlg dlg(mLimits.minY_mm, mLimits.maxY_mm);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cNewSpidercam_Y_PositionDlg::positionUpdated);
    }

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int y_mm = dlg.y_mm();

    if (y_mm < mLimits.minY_mm) y_mm = mLimits.minY_mm;
    if (y_mm > mLimits.maxY_mm) y_mm = mLimits.maxY_mm;

    child->set_Y_Position(y_mm);

    child->reloadPath();
}

void cMainWindow::onEditMoveMeasurementZ()
{
    cNewSpidercam_Z_PositionDlg dlg(mLimits.minHeight_mm, mLimits.maxHeight_mm);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cNewSpidercam_Z_PositionDlg::positionUpdated);
    }

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
        return;

    auto* childSubWindow = mpMdiArea->currentSubWindow();
    if (!childSubWindow)
        return;

    auto* child = static_cast<cExperimentDesignMdiChild*>(childSubWindow->widget());

    int z_mm = dlg.z_mm();

    if (z_mm < mLimits.minHeight_mm) z_mm = mLimits.minHeight_mm;
    if (z_mm > mLimits.maxHeight_mm) z_mm = mLimits.maxHeight_mm;

    child->set_Z_Position(z_mm);

    child->reloadPath();
}

void cMainWindow::onEditShiftMeasurement()
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
    connect(&dlg, &cCreateLidarExperimentFromSpiderCamDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cCreateLidarExperimentFromSpiderCamDlg::positionUpdated);
    }

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
    connect(&dlg, &cCreateLidarExperimentFromGpsDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

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
    connect(&dlg, &cCreateLidarExperimentFromPlotInfoDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

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
    connect(&dlg, &cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::positionUpdated);
    }

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
    cCreateHyperspectralExperimentFromSpiderCamDlg dlg;

    connect(&dlg, &cCreateHyperspectralExperimentFromSpiderCamDlg::clearPaths,          mpFieldLayout, &cFieldLayoutWidget::clearRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromSpiderCamDlg::drawPath,            mpFieldLayout, &cFieldLayoutWidget::drawRecordingPath);
    connect(&dlg, &cCreateHyperspectralExperimentFromSpiderCamDlg::experimentChanged,   this,          &cMainWindow::onMeasurementChange);

    if (mpModel && mpModel->isConnected())
    {
        dlg.positionUpdated(mpModel->getPosition());
        connect(mpModel, &cPlannerDataModel::positionChanged, &dlg, &cCreateHyperspectralExperimentFromSpiderCamDlg::positionUpdated);
    }

    auto result = dlg.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }

    mpEditMenu->setDisabled(false);
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
    connect(&dlg, &cCreateHyperspectralExperimentFromGpsDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

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
    connect(&dlg, &cCreateHyperspectralExperimentFromPlotInfoDlg::experimentChanged, this, &cMainWindow::onMeasurementChange);

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

void cMainWindow::onPreferenceDefaultMeasurementDirectory()
{
    QString defaultDirectory = mSettings.value("Defaults/measurementDirectory").toString();

    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Default Directory for Saving/Loading Measurement Files..."), defaultDirectory);

    if (directory.isEmpty())
        return;

    mSettings.setValue("Defaults/measurementDirectory", directory);
    mMeasurementFilesPath = directory;

    emit defaultMeasurementPathChange(mMeasurementFilesPath);
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
 * Slots associated with "Connect" menu actions
 *******************************************************************/
void cMainWindow::onConnectToSpidercam()
{
    if (mpModel)
    {
        onDisconnectFromSpidercam();
        return;
    }

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

    auto widgets = create_experiment_controller(name, ctrlDoc, true);

    cExperimentControlModel* pCtrlModel = widgets.pModel;

    if ((pCtrlModel == nullptr))
    {
        return;
    }

    mpModel = new cPlannerDataModelLocal(this);

    QObject::connect(mpModel, &cPlannerDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(mpModel, &cPlannerDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(mpModel, &cPlannerDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(mpModel, &cPlannerDataModel::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::connect(mpModel, &cPlannerDataModel::connectedToController, this, &cMainWindow::connectedToController);
    QObject::connect(mpModel, &cPlannerDataModel::disconnectedFromController, this, &cMainWindow::disconnectedFromController);

    QObject::connect(mpModel, &cPlannerDataModel::limitsChanged, mpFieldLayout, &cFieldLayoutWidget::updateLimits);
    QObject::connect(mpModel, &cPlannerDataModel::positionChanged, mpFieldLayout, &cFieldLayoutWidget::updatePosition);
    QObject::connect(mpModel, &cPlannerDataModel::recordingStateChanged, mpFieldLayout, &cFieldLayoutWidget::updateRecordingState);
    QObject::connect(mpModel, &cPlannerDataModel::experimentStatus, mpFieldLayout, &cFieldLayoutWidget::experimentStatusUpdating);
    QObject::connect(mpModel, &cPlannerDataModel::experimentStateChanged, mpFieldLayout, &cFieldLayoutWidget::experimentStateChanging);

    QObject::connect(mpModel, &cPlannerDataModel::experimentTerminated, this, &cMainWindow::onMeasurementTerminated);
    QObject::connect(mpModel, &cPlannerDataModel::experimentCompleted, this, &cMainWindow::onMeasurementCompleted);

    QObject::connect(pCtrlModel, &cExperimentControlModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(pCtrlModel, &cExperimentControlModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(pCtrlModel, &cExperimentControlModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(pCtrlModel, &cExperimentControlModel::errorMessage, this, &cMainWindow::onErrorMessage);


    //    QObject::connect(this, &cMainWindow::refreshDisplay, mpController, &cExperimentControlView::refresh);

    /*
        QObject::connect(pModel, &cExperimentControlModel::experimentStatus,
            mpController, &cExperimentControlView::experimentStatusUpdating);

        QObject::connect(pModel, &cExperimentControlModel::experimentStateChanged,
            mpController, &cExperimentControlView::experimentStateChanging);
    */

    mpModel->addExperimentControlModel(pCtrlModel);

    if (configDoc.contains(name))
    {
        pCtrlModel->configure(configDoc[name]);
    }

    mpModel->startDataThread();

    mpSpidercamConnect->setText(tr("Disconnect"));
    mpSpidercamConnect->setStatusTip(tr("Disconnect from Spidercam"));

    mpTestMeasurement->setEnabled(true);
    mpStopMeasurement->setEnabled(true);
    mpPauseRunMeasurement->setEnabled(true);
}


void cMainWindow::onDisconnectFromSpidercam()
{
    if (!mpModel)
    {
        return;
    }

    onSpidercamStopMeasurement();

    mpTestMeasurement->setEnabled(false);
    mpStopMeasurement->setEnabled(false);
    mpPauseRunMeasurement->setEnabled(false);

    mpModel->stopDataThread();

    cExperimentControlModel* pCtrlModel = mpModel->removeExperimentControlModel();

    QObject::disconnect(pCtrlModel, &cExperimentControlModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::disconnect(pCtrlModel, &cExperimentControlModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::disconnect(pCtrlModel, &cExperimentControlModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::disconnect(pCtrlModel, &cExperimentControlModel::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::disconnect(mpModel, &cPlannerDataModel::limitsChanged, mpFieldLayout, &cFieldLayoutWidget::updateLimits);
    QObject::disconnect(mpModel, &cPlannerDataModel::positionChanged, mpFieldLayout, &cFieldLayoutWidget::updatePosition);
    QObject::disconnect(mpModel, &cPlannerDataModel::recordingStateChanged, mpFieldLayout, &cFieldLayoutWidget::updateRecordingState);
    QObject::disconnect(mpModel, &cPlannerDataModel::experimentStatus, mpFieldLayout, &cFieldLayoutWidget::experimentStatusUpdating);
    QObject::disconnect(mpModel, &cPlannerDataModel::experimentStateChanged, mpFieldLayout, &cFieldLayoutWidget::experimentStateChanging);

    QObject::disconnect(mpModel, &cPlannerDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::disconnect(mpModel, &cPlannerDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::disconnect(mpModel, &cPlannerDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::disconnect(mpModel, &cPlannerDataModel::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::disconnect(mpModel, &cPlannerDataModel::experimentTerminated, this, &cMainWindow::onMeasurementTerminated);
    QObject::disconnect(mpModel, &cPlannerDataModel::experimentCompleted, this, &cMainWindow::onMeasurementCompleted);

    delete mpModel;
    mpModel = nullptr;

    delete pCtrlModel;

    mpSpidercamConnect->setText(tr("Connect"));
    mpSpidercamConnect->setStatusTip(tr("Connect to Spidercam"));

    emit disconnectedFromController();
}

void cMainWindow::onSpidercamTestMeasurement()
{
    if (!mpModel)
    {
        return;
    }

    if (!mpModel->systemReady())
    {
        return;
    }

    auto* pMeasurement = static_cast<cMeasurementTreeItem*>(mpMeasurements->currentItem());

    if (pMeasurement == nullptr)
    {
        return;
    }

    if (pMeasurement->hasMeasurementDocument())
    {
        loadMeasurement(*pMeasurement);
    }

    if (!mpModel->isExperimentLoaded())
    {
        return;
    }

    if (mpModel->experimentRequiresDataFile())
    {
        std::string fileName = mpModel->measurementTitle();
        if (!mpModel->openDataFile("", fileName, false))
        {
            mpModel->terminateExperiment();
            return;
        }
    }

    mpModel->startExperiment();

    mpPauseRunMeasurement->setText(tr("Pause Measurement"));
    mpPauseRunMeasurement->setStatusTip(tr("Pause the running measurement"));

    emit measurementRunning();
}

void cMainWindow::onSpidercamStopMeasurement()
{
    if (!mpModel)
    {
        return;
    }

    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    mpModel->terminateExperiment();

    mpPauseRunMeasurement->setText(tr("Pause Measurement"));
    mpPauseRunMeasurement->setStatusTip(tr("Pause the running measurement"));
}

void cMainWindow::onSpidercamPauseRunMeasurement()
{
    if (!mpModel)
    {
        return;
    }

    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    if (mpModel->isExperimentPaused())
    {
        mpModel->startExperiment();
        mpPauseRunMeasurement->setText(tr("Pause Measurement"));
        mpPauseRunMeasurement->setStatusTip(tr("Pause the running measurement"));
    }
    else
    {
        mpModel->pauseExperiment();
        mpPauseRunMeasurement->setText("Continue Measurement");
        mpPauseRunMeasurement->setStatusTip(tr("Continue the running measurement"));
    }
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
void cMainWindow::onOpenMeasurement(const QString& filename)
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
void cMainWindow::onMeasurementChange(QSharedPointer<cExperimentFile> measurement)
{
    if (measurement.isNull()) return;

    auto filename = measurement->getFileName();
    auto title = measurement->getMeasurementName();

    if (title.empty())
        title = measurement->getExperimentName();

    auto list = mpMdiArea->subWindowList();
    for (auto* subWindow : list)
    {
        auto* child = static_cast<cExperimentDesignMdiChild*>(subWindow->widget());
        if ((!title.empty() && (child->getMeasurementTitle() == title))
            || (!filename.empty() && (child->getFileName() == filename)))
        {
            child->setMeasurementFile(*measurement);
            mpMdiArea->setActiveSubWindow(subWindow);
            return;
        }
    }
    auto* child = createMdiChild();
    child->newFile(*measurement);
    child->show();

    mpEditMenu->setDisabled(false);
}

//-----------------------------------------------------------------------------
void cMainWindow::onMeasurementListUpdateNeeded()
{
    mpMeasurements->reloadMeasurements();
}


//-----------------------------------------------------------------------------
bool cMainWindow::loadMeasurement(const cMeasurementTreeItem& measurement)
{
    QString msg = "Loading measurement \"";
    msg += measurement.text(0);
    msg += "\" from file ";
    msg += measurement.getFilename();
    onStatusUpdate(msg);

    std::string name = measurement.text(0).toStdString();
    auto measureDoc = measurement.getMeasurementDocument();
    if (!mpModel->loadExperiment(name, measureDoc))
    {
        return false;
    }

    return true;
}

bool cMainWindow::loadMeasurement(const std::filesystem::path& measurement_file)
{
    using namespace nlohmann;

    std::ifstream in;
    in.open(measurement_file);

    if (!in.is_open())
    {
        throw std::invalid_argument("Could not open file.");
    }

    nlohmann::json jsonDoc;
    std::string name;

    try
    {
        in >> jsonDoc;

        if (jsonDoc.contains("measurement name"))
            name = jsonDoc["measurement name"];
        else if (jsonDoc.contains("measurement_name"))
            name = jsonDoc["measurement_name"];
        else if (jsonDoc.contains("experiment name"))
            name = jsonDoc["experiment name"];
        else
            name = jsonDoc["experiment_name"];
    }
    catch (const detail::exception& e)
    {
        QString msg = "Failed to loading measurement file: ";
        msg += QString::fromStdString(measurement_file.string());
        onWarningMessage("File Error", msg);

        return false;
    }

    QString msg = "Loading measurement \"";
    msg += QString::fromStdString(name);
    msg += "\" from file ";
    msg += QString::fromStdString(measurement_file.string());
    onStatusUpdate(msg);

    if (!mpModel->loadExperiment(name, jsonDoc))
    {
        QString msg = "Measurement \"";
        msg += QString::fromStdString(name);
        msg += "\" from file ";
        msg += QString::fromStdString(measurement_file.string());
        msg += " failed to load!";
        onStatusUpdate(msg);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void cMainWindow::onMeasurementRun(const QString& filename)
{
    if (!mpModel)
        return;

    if (mpModel->isExperimentRunning())
    {
        if (mpModel->isExperimentPaused())
        {
            mpModel->startExperiment();
        }
        return;
    }

    onOpenMeasurement(filename);

    if (!mpModel->systemReady())
    {
        return;
    }

    auto* pMeasurement = static_cast<cMeasurementTreeItem*>(mpMeasurements->currentItem());

    if (pMeasurement == nullptr)
    {
        return;
    }

    if (pMeasurement->hasMeasurementDocument())
    {
        loadMeasurement(*pMeasurement);
    }
 
    if (!mpModel->isExperimentLoaded())
    {
        return;
    }

    if (mpModel->experimentRequiresDataFile())
    {
        std::string fileName = mpModel->measurementTitle();
        if (!mpModel->openDataFile("", fileName, false))
        {
            mpModel->terminateExperiment();
            return;
        }
    }

    mpModel->startExperiment();

    emit measurementRunning();
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

