
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include "AboutDialog.hpp"

#include "Constants.hpp"

#include "GpsFileReader.hpp"

//#include "FieldUtils.hpp"
//#include "ExportUtils.hpp"
//#include "KinematicUtils.hpp"

//#include "vtkGpsToSpiderCamPolyData.hpp"

//#include "ViewToolbar.hpp"

#include "RappTriangle.hpp"

#include "StringUtils.hpp"

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/BlockDataFileExceptions.hpp>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QIODevice>
#include <QTextStream>

//#include <vtkNamedColors.h>
//#include <vtkNew.h>
//#include <vtkPointData.h>
//#include <vtkDoubleArray.h>
//#include <vtkCellData.h>
//#include <vtkIdList.h>
//#include <vtkTable.h>
//#include <vtkDataSetReader.h>
//#include <vtkDelimitedTextReader.h>
//#include <vtkTableToPolyData.h>
//#include <vtkDelaunay2D.h>
//#include <vtkLookupTable.h>
//#include <vtkMinimalStandardRandomSequence.h>
//#include <vtkActor.h>
//#include <vtkDataSetMapper.h>
//#include <vtkProperty.h>
//#include <vtkGenericOpenGLRenderWindow.h>

#include <numbers>
#include <format>


using namespace std::filesystem;


namespace
{
    double wrap_neg_pi_to_pi(double angle_rad)
    {
        while (angle_rad > std::numbers::pi)
        {
            angle_rad -= 2.0 * std::numbers::pi;
        }

        while (angle_rad < -std::numbers::pi)
        {
            angle_rad += 2.0 * std::numbers::pi;
        }

        return angle_rad;
    }

    double wrap_neg_180_to_180(double angle_deg)
    {
        while (angle_deg > 180.0)
        {
            angle_deg -= 360.0;
        }

        while (angle_deg < -180.0)
        {
            angle_deg += 360.0;
        }

        return angle_deg;
    }
}

cMainWindow::cMainWindow(QWidget* parent)
    : QMainWindow(parent), mpUI(new Ui::cMainWindow), mSettings("UIUC", "Ceres Field Mapper")
{
    mpUI->setupUi(this);
}

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
    createStatusBar();

    createDockWindows();

    restoreGeometry(mSettings.value("mainWindow/geometry").toByteArray());
    restoreState(mSettings.value("mainWindow/windowState").toByteArray());

//    connect(&mFieldData, &cRappFieldDataModel::groundPointsUpdated, this, &cMainWindow::onGroundPointsUpdate);
//    connect(&mFieldData, &cRappFieldDataModel::aerialPointsUpdated, this, &cMainWindow::onAerialPointsUpdate);
//    connect(&mFieldData, &cRappFieldDataModel::scanLoadComplete, this, &cMainWindow::onScanLoadComplete);
//    connect(&mFieldData, &cRappFieldDataModel::onErrorMessage, this, &cMainWindow::onStatusUpdate);

    QString fileName = mSettings.value("Defaults/groundMeshFile").toString();

    if (!fileName.isEmpty())
        LoadGroundReferenceData(fileName);

    fileName = mSettings.value("Defaults/aerialMeshFile").toString();

    if (!fileName.isEmpty())
        LoadAerialData(fileName);
}

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu      = mpUI->menubar->addMenu(tr("&File"));
    mpComputeMenu   = mpUI->menubar->addMenu(tr("&Compute"));
    mpViewMenu      = mpUI->menubar->addMenu(tr("&View"));
    mpSettingMenu   = mpUI->menubar->addMenu(tr("&Setting"));
    mpHelpMenu      = mpUI->menubar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Sub Menu
    pMenuItem = new QAction(tr("Load Aerial Mesh"), this);
    pMenuItem->setStatusTip(tr("Load aerial mesh data into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileLoadAerialMesh);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Load Ground Mesh"), this);
    pMenuItem->setStatusTip(tr("Load ground mesh data into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileLoadGroundMesh);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Load Field Scan"), this);
    pMenuItem->setStatusTip(tr("Load a field scan into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileLoadFieldScan);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Load and Process Field Scans"), this);
    pMenuItem->setStatusTip(tr("Load a field scans into memory"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileLoadFieldScans);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("Save the aerial mesh data"), this);
    pMenuItem->setStatusTip(tr("Save the  aerial mesh data"));
    pMenuItem->setEnabled(false);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveAerialMesh);
//    connect(&mFieldData, &cRappFieldDataModel::enableSaveAerialPoints, pMenuItem, &QAction::setEnabled);
//    connect(this, &cMainWindow::enableSavePointCloud, pMenuItem, &QAction::setEnabled);
    mpFileMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Save the ground mesh data"), this);
    pMenuItem->setStatusTip(tr("Save the  ground mesh data"));
    pMenuItem->setEnabled(false);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileSaveGroundMesh);
//    connect(&mFieldData, &cRappFieldDataModel::enableSaveGroundPoints, pMenuItem, &QAction::setEnabled);
//    connect(this, &cMainWindow::enableSavePointCloud, pMenuItem, &QAction::setEnabled);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    QMenu* exportMenu = mpFileMenu->addMenu(tr("&Export..."));
    exportMenu->setEnabled(true);
//    connect(this, &cMainWindow::enableSavePointCloud, exportMenu, &QMenu::setEnabled);

    pMenuItem = new QAction(tr("Aerial Mesh data"), this);
    pMenuItem->setStatusTip(tr("Export the aerial mesh data to a PLY files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileExportAerialMeshAsPlyFile);
    exportMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Ground Mesh data"), this);
    pMenuItem->setStatusTip(tr("Export the ground mesh data to a PLY files"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileExportGroundMeshAsPlyFile);
    exportMenu->addAction(pMenuItem);
   

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(QIcon(":/quit.png"), tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);


    // Build the Compute Sub Menu

    pMenuItem = new QAction(tr("Compute Dolly Path (SpiderCam)"), this);
    pMenuItem->setStatusTip(tr("Compute dolly path based on SpiderCam data."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onComputeDollyPathSpiderCam);
    mpComputeMenu->addAction(pMenuItem);

    mpComputeMenu->addSeparator();

    pMenuItem = new QAction(tr("Compute Aerial Mesh"), this);
    pMenuItem->setStatusTip(tr("Compute ."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onComputeAerialMesh);
    mpComputeMenu->addAction(pMenuItem);

    mpComputeMenu->addSeparator();

    pMenuItem = new QAction(tr("Clear Aerial Mesh"), this);
    pMenuItem->setStatusTip(tr("Clear the aerial points and mesh."));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onComputeClearAerialMesh);
    mpComputeMenu->addAction(pMenuItem);


    // Build the View Sub Menu
    pMenuItem = new QAction(tr("Ground Points"), this);
    pMenuItem->setStatusTip(tr("View the ground point mesh"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onViewGroundPoints);
    pMenuItem->setCheckable(true);
    pMenuItem->setChecked(mShowGroundPoints);
    mpViewMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Ground Mesh"), this);
    pMenuItem->setStatusTip(tr("View the triangular ground mesh"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onViewGroundMesh);
    pMenuItem->setCheckable(true);
    pMenuItem->setChecked(mShowGroundMesh);
    mpViewMenu->addAction(pMenuItem);

    mpViewMenu->addSeparator();

    mpViewAerialMesh = new QAction(tr("Aerial Points"), this);
    mpViewAerialMesh->setStatusTip(tr("View the aerial point mesh"));
    connect(mpViewAerialMesh, &QAction::triggered, this, &cMainWindow::onViewAerialPoints);
    mpViewAerialMesh->setCheckable(true);
    mpViewAerialMesh->setChecked(mShowAerialPoints);
    mpViewMenu->addAction(mpViewAerialMesh);

    mpViewAerialMesh = new QAction(tr("Aerial Mesh"), this);
    mpViewAerialMesh->setStatusTip(tr("View the triangular aerial mesh"));
    connect(mpViewAerialMesh, &QAction::triggered, this, &cMainWindow::onViewAerialMesh);
    mpViewAerialMesh->setCheckable(true);
    mpViewAerialMesh->setChecked(mShowAerialMesh);
    mpViewMenu->addAction(mpViewAerialMesh);

    mpViewMenu->addSeparator();

    mpViewDollyPathSpiderCam = new QAction(tr("View Dolly Path (SpiderCam)"), this);
    mpViewDollyPathSpiderCam->setStatusTip(tr("View dolly path based on SpiderCam data."));
    mpViewDollyPathSpiderCam->setEnabled(false);
    mpViewDollyPathSpiderCam->setCheckable(true);
    connect(mpViewDollyPathSpiderCam, &QAction::triggered, this, &cMainWindow::onViewDollyPathSpiderCam);
    mpViewMenu->addAction(mpViewDollyPathSpiderCam);

    mpViewDollyPathGPS = new QAction(tr("View Dolly Path (GPS)"), this);
    mpViewDollyPathGPS->setStatusTip(tr("View dolly path based on GPS data."));
    mpViewDollyPathGPS->setEnabled(false);
    mpViewDollyPathGPS->setCheckable(true);
    connect(mpViewDollyPathGPS, &QAction::triggered, this, &cMainWindow::onViewDollyPathGPS);
    mpViewMenu->addAction(mpViewDollyPathGPS);

    mpViewDollyPathComputed = new QAction(tr("View Dolly Path (Computed)"), this);
    mpViewDollyPathComputed->setStatusTip(tr("View the computed dolly path based on SpiderCam and GPS data."));
    mpViewDollyPathComputed->setEnabled(false);
    mpViewDollyPathComputed->setCheckable(true);
    connect(mpViewDollyPathComputed, &QAction::triggered, this, &cMainWindow::onViewDollyPathComputed);
    mpViewMenu->addAction(mpViewDollyPathComputed);


    // Build the Settings Sub Menu
    pMenuItem = new QAction(tr("Load Ground Data"), this);
    pMenuItem->setStatusTip(tr("Load the GPS based ground data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onSettingsLoadGroundMesh);
    mpSettingMenu->addAction(pMenuItem);

    mpSettingMenu->addSeparator();

    // Build the Help Sub Menu
    pMenuItem = new QAction(QIcon(":/about.png"), tr("&About"), this);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::showAboutDialog);
    mpHelpMenu->addAction(pMenuItem);
}

//-----------------------------------------------------------------------------
void cMainWindow::createActions()
{
}

//-----------------------------------------------------------------------------
void cMainWindow::createToolBars()
{
//    auto* toolbar = new cViewToolbar(this);

    //connect(toolbar, &cViewToolbar::zoomToExtent, mpUI->SceneWidget, &cSceneWidget::zoomToExtent);
    //connect(toolbar, &cViewToolbar::topView, mpUI->SceneWidget, &cSceneWidget::topView);
    //connect(toolbar, &cViewToolbar::bottomView, mpUI->SceneWidget, &cSceneWidget::bottomView);
    //connect(toolbar, &cViewToolbar::rightView, mpUI->SceneWidget, &cSceneWidget::rightView);
    //connect(toolbar, &cViewToolbar::leftView, mpUI->SceneWidget, &cSceneWidget::leftView);
    //connect(toolbar, &cViewToolbar::frontView, mpUI->SceneWidget, &cSceneWidget::frontView);
    //connect(toolbar, &cViewToolbar::backView, mpUI->SceneWidget, &cSceneWidget::backView);

    //addToolBar(toolbar);
}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows()
{
}

void cMainWindow::closeEvent(QCloseEvent* event)
{
    mSettings.setValue("mainWindow/geometry", saveGeometry());
    mSettings.setValue("mainWindow/windowState", saveState());
    QMainWindow::closeEvent(event);
}

//-----------------------------------------------------------------------------
void cMainWindow::onStatusUpdate(QString text)
{
    statusBar()->showMessage(text);
}

//-----------------------------------------------------------------------------
void cMainWindow::showAboutDialog()
{
    AboutDialog().exec();
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileLoadFieldScan()
{
    QString defaultDirectory = mSettings.value("Defaults/dataDirectory").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Load file"), defaultDirectory,
        "Ceres Files (*.ceres)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Open file
    QFileInfo fileInfo(fileName);

    if (!fileInfo.exists())
        return;

    mFileName = fileInfo.completeBaseName();

    emit enableSavePointCloud(false);

//    mFieldData.loadFieldScanData(fileName.toStdString());

//    mpUI->SceneWidget->reset();
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileLoadFieldScans()
{
    QString defaultDirectory = mSettings.value("Defaults/dataDirectory").toString();

    QString dirName = QFileDialog::getExistingDirectory(this, tr("Load files"), defaultDirectory);

    // Return on Cancel
    if (dirName.isEmpty())
        return;

/*
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Load files"), defaultDirectory);

    // Return on Cancel
    if (fileNames.isEmpty())
        return;
*/

    const std::filesystem::path source_dir = dirName.toStdString();

//    cLoadScansConfirmDlg* pDlg = new cLoadScansConfirmDlg(this);

//    pDlg->initialize(source_dir);

//    auto result = pDlg->exec();
//    if (result == QDialog::Rejected)
//        return;

//    mBatchProcess = pDlg->getSelectedScans();

    if (mBatchProcess.empty())
        return;

//    mFileName = fileInfo.completeBaseName();

    emit enableSavePointCloud(false);

//    mFieldData.loadFieldScanData(mBatchProcess.front().string());

    mSettings.setValue("Defaults/dataDirectory", dirName);
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileSaveAerialMesh()
{
    using namespace nStringUtils;

//    if (mFieldData.getAerialPoints().empty())
//        return;

/*
    QString defaultDirectory = mSettings.value("Defaults/pointcloudDirectory").toString();

    defaultDirectory += "/";
    defaultDirectory += out_filename;
*/
    QString defaultDirectory;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), defaultDirectory,
        "CSV Files (*.csv)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Truncate | QIODevice::OpenModeFlag::Text))
    {
        QTextStream stream(&file);

        stream << "SPDR,0,0,6000,SPDR\n";

//        auto data = mFieldData.getAerialPoints();

//        for (int i = 0; i < data.size(); ++i)
//        {
//            auto point = data[i];

//            stream << "AP_" << QString::fromStdString(std::format("{:04}", i)) << ", ";
//            stream << point.x_mm << ", ";
//            stream << point.y_mm << ", ";
//            stream << point.z_mm << ",\n";
//        }
    }

    file.close();
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileSaveGroundMesh()
{
    /*
        using namespace nStringUtils;

        auto fe = removeProcessedTimestamp(mFileName.toStdString());

        QString out_filename = QString::fromStdString(addProcessedTimestamp(fe.filename));

        out_filename += ".ceres";

        QString defaultDirectory = mSettings.value("Defaults/pointcloudDirectory").toString();

        defaultDirectory += "/";
        defaultDirectory += out_filename;

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), defaultDirectory,
            "Ceres Files (*.ceres)");

        // Return on Cancel
        if (fileName.isEmpty())
            return;

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();

        auto pcg = mData.getOusterInfo()->getPointCloudGenerator().lock();

        auto* saver = new cPointCloudSaver(pcg->getPointCloud(), true);

        auto range_m = pcg->getValidRange_m();
        saver->setRangeWindow_m(range_m.first, range_m.second);

        auto azimuth_deg = pcg->getAzimuthWindow_deg();
        saver->setAzimuthWindow_deg(azimuth_deg.first, azimuth_deg.second);

        auto altitude_deg = pcg->getAltitudeWindow_deg();
        saver->setAltitudeWindow_deg(altitude_deg.first, altitude_deg.second);

        auto dolly_path = pcg->getComputedDollyPath();
        saver->setKinematicModel(mKinematicModel, dolly_path);

        saver->setInputFile(mData.getFileName());
        saver->setOutputFile(fileName.toStdString());

        QString str = "Saving Point Cloud...";

        QProgressDialog* progress = new QProgressDialog(str, "Abort", 0, 100, this, Qt::WindowStaysOnTopHint);
        progress->setWindowTitle("RAPP LiDAR Mapper");
        progress->setFixedWidth(300);
        progress->setWindowModality(Qt::WindowModal);
        progress->setMinimumDuration(0);

        connect(progress, &QProgressDialog::canceled, saver, &cPointCloudSaver::abort);
        connect(saver, &cPointCloudSaver::progressUpdated, progress, &QProgressDialog::setValue);
        connect(saver, &cPointCloudSaver::savingingComplete, progress, &QObject::deleteLater);
        connect(saver, &cPointCloudSaver::savingingComplete, saver, &QObject::deleteLater);

        QApplication::restoreOverrideCursor();

        saver->start();
    */
}

void cMainWindow::onFileExportAerialMeshAsPlyFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Aerial Data"), "",
        "PLY Files (*.ply)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    std::string fn = fileName.toStdString();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    try
    {
//        exportMesh(fn, mFieldData.getAerialPoints(), false);
    }
    catch (const std::exception& e)
    {

    }

    QApplication::restoreOverrideCursor();
}

void cMainWindow::onFileExportGroundMeshAsPlyFile()
{
    /*
        using namespace nStringUtils;

        auto fe = removeProcessedTimestamp(mFileName.toStdString());

        QString out_filename = QString::fromStdString(addProcessedTimestamp(fe.filename));

        out_filename += ".ply";

        QString defaultDirectory = mSettings.value("Defaults/exportDirectory").toString();
        defaultDirectory += "/";
        defaultDirectory += out_filename;

        QString fileName = QFileDialog::getSaveFileName(this, tr("Export file"), defaultDirectory,
            "PLY Files (*.ply)");

        // Return on Cancel
        if (fileName.isEmpty())
            return;

        std::string fn = fileName.toStdString();

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();

        try
        {
            auto ouster = mData.getOusterInfo();
            auto pcg = ouster->getPointCloudGenerator().lock();
            exportPointcloud2Ply(fn, pcg->getPointCloud());
        }
        catch (const std::exception& e)
        {

        }

        QApplication::restoreOverrideCursor();
    */
}

//-----------------------------------------------------------------------------
void cMainWindow::onScanLoadComplete()
{
//    if (mFieldData.fieldScanLoadAborted())
//    {
//        mBatchProcess.clear();
//        return;
//    }

    // Process data here
    onComputeAerialMesh();

    if (mBatchProcess.size() > 0)
        mBatchProcess.erase(mBatchProcess.begin());

    if (mBatchProcess.empty())
        return;

    //mFieldData.clear();
    //mFieldData.clearPointclouds();

    //std::string filename = mBatchProcess.front().string();
    //auto result = mFieldData.loadFieldScanData(filename);

//    if (result == cRappFieldDataModel::eBUSY)
//    {
//        QTimer::singleShot(5000, [this, &result, filename]()
//            {
//                result = mFieldData.loadFieldScanData(filename);
//            }
//        );
//    }

//    else if (result == cRappFieldDataModel::eFILE_NOT_FOUND)
//    {
//        if (mBatchProcess.size() > 0)
//        {
//            mBatchProcess.erase(mBatchProcess.begin());
//            filename = mBatchProcess.front().string();
//            result = mFieldData.loadFieldScanData(filename);
//        }
//    }
}

//-----------------------------------------------------------------------------
void cMainWindow::onComputeDollyPathSpiderCam()
{
//    auto spiderCam = mFieldData.getSpiderCamInfo();
//    if (!spiderCam)
//        return;
//
//    if (spiderCam->empty())
//        return;
//
//    //auto data = computeDollyKinematics(spiderCam->getPositionData(), spiderCam->getStartIndex(), spiderCam->getEndIndex());
//
////    mpUI->SceneWidget->addDollyPath_SpiderCam(data);
//
//    mpViewDollyPathSpiderCam->setEnabled(true);
//    mpViewDollyPathSpiderCam->setChecked(true);
}

//-----------------------------------------------------------------------------
void cMainWindow::onComputeDollyPathGPS(int device_id)
{
    //auto ssnx = mFieldData.getSsnxInfo();
    //if (!ssnx)
    //    return;

    //auto gps = ssnx->getGeodeticPositions(device_id);

    //if (gps.empty())
    //    return;

//    auto data = computeDollyKinematics(gps);
//
//
//    std::vector<rfm::rappPoint_t> rapp_points;
//
//    for (const auto& pos : data)
//    {
//        rfm::rappPoint_t point = { static_cast<int32_t>(pos.x_mm), static_cast<int32_t>(pos.y_mm), static_cast<int32_t>(pos.z_mm) };
//        rapp_points.push_back(point);
//    }
//
//    mFieldData.addAerialPoints(rapp_points);
//
//    auto aerial_data = mFieldData.getAerialPoints();
//    auto mesh = computeDelaunay2dMesh(aerial_data);
//
//    mFieldData.clearAerialMesh();
//    mFieldData.addAerialMeshData(mesh);
//
////    mpUI->SceneWidget->addDollyPath_GPS(data);
//
//    mpViewDollyPathGPS->setEnabled(true);
//    mpViewDollyPathGPS->setChecked(true);
}

//-----------------------------------------------------------------------------
void cMainWindow::onComputeAerialMesh()
{
    //auto ssnx = mFieldData.getSsnxInfo();
    //if (!ssnx)
    //    return;

    //auto gps = ssnx->getGeodeticPositions(0);

    //if (gps.empty())
    //    return;

    //auto data = computeDollyKinematics(gps);

    //int32_t gps_x_mm = -60;
    //int32_t gps_y_mm = 0;
    //int32_t gps_z_mm = -20;

    //auto start = data.front();
    //auto end = data.back();
    //rfm::rappPoint2D_t start_pos = { static_cast<int32_t>(start.x_mm), static_cast<int32_t>(start.y_mm) };
    //rfm::rappPoint2D_t end_pos = { static_cast<int32_t>(end.x_mm), static_cast<int32_t>(end.y_mm) };

    //int n = 1;
    //rfm::rappPoint_t point = { static_cast<int32_t>(start.x_mm - gps_x_mm), static_cast<int32_t>(start.y_mm - gps_y_mm), static_cast<int32_t>(start.z_mm - gps_z_mm) };

    //std::vector<rfm::rappPoint_t> rapp_points;

    //double avg_band_mm = 200;
    //double min_distance_mm = 10000 - (avg_band_mm / 2);

    //for (const auto& pos : data)
    //{
    //    auto dx = start_pos.x_mm - pos.x_mm;
    //    auto dy = start_pos.y_mm - pos.y_mm;

    //    double d = sqrt(dx*dx + dy*dy);

    //    if (d < avg_band_mm)
    //    {
    //        point.x_mm += (pos.x_mm - gps_x_mm);
    //        point.y_mm += (pos.y_mm - gps_y_mm);
    //        point.z_mm += (pos.z_mm - gps_z_mm);
    //        ++n;
    //    }
    //    else if (d > min_distance_mm)
    //    {
    //        start_pos.x_mm = pos.x_mm;
    //        start_pos.y_mm = pos.y_mm;

    //        point.x_mm = (pos.x_mm - gps_x_mm);
    //        point.y_mm = (pos.y_mm - gps_y_mm);
    //        point.z_mm = (pos.z_mm - gps_z_mm);

    //        n = 1;
    //    }
    //    else
    //    {
    //        if (n > 0)
    //        {
    //            point.x_mm /= n;
    //            point.y_mm /= n;
    //            point.z_mm /= n;

    //            rapp_points.push_back(point);
    //        }

    //        n = 0;
    //    }
    //}

    //mFieldData.addAerialScanData(rapp_points);

    //auto aerial_data = mFieldData.getAerialPoints();
    //auto mesh = computeDelaunay2dMesh(aerial_data);

    //mFieldData.clearAerialMesh();
    //mFieldData.addAerialMeshData(mesh);

//    mpUI->SceneWidget->addDollyPath_GPS(data);

}

//-----------------------------------------------------------------------------
void cMainWindow::onComputeClearAerialMesh()
{
    //auto result = QMessageBox::warning(this, "Warning",
    //                "Are you sure you want to clear the aerial data?",
    //                QMessageBox::Ok | QMessageBox::Cancel);

    //if (result == QMessageBox::Cancel)
    //    return;

    //mFieldData.clearAerialMesh();
    //mFieldData.clearAerialPoints();
    //mpUI->SceneWidget->removeAerialMesh();
}

//-----------------------------------------------------------------------------
void cMainWindow::showOpenFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "",
        "VTK Files (*.vtk)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Test to make sure the file exists file
    if (!std::filesystem::exists(fileName.toStdString()))
        return;

    openFile(fileName);
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileLoadAerialMesh()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import file"), "",
        "Aerial CSV Data Files (*.csv)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Test to make sure the file exists file
    if (!std::filesystem::exists(fileName.toStdString()))
        return;

    cGpsFileReader gps;
    gps.loadFromFile(fileName.toStdString());

    //auto rapp_points = gps.GetRappPoints();

    //mFieldData.addAerialPoints(rapp_points);

    //auto data = mFieldData.getAerialPoints();
    //auto mesh = computeDelaunay2dMesh(data);

    //mFieldData.clearAerialMesh();
    //mFieldData.addAerialMeshData(mesh);
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileLoadGroundMesh()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import file"), "",
        "Ground CSV Data Files (*.csv)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Test to make sure the file exists file
    if (!std::filesystem::exists(fileName.toStdString()))
        return;

    cGpsFileReader gps;
    gps.loadFromFile(fileName.toStdString());

    //auto rapp_points = gps.GetRappPoints();

    //mFieldData.useGroundHeight();
    //mFieldData.addGroundPoints(rapp_points);

    //auto data = mFieldData.getGroundPoints();
    //auto mesh = computeDelaunay2dMesh(data);

    //mFieldData.clearGroundMesh();
    //mFieldData.addGroundMeshData(mesh);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewGroundPoints()
{
    mShowGroundPoints = !mShowGroundPoints;
    //mpUI->SceneWidget->setGroundMeshVisibility(mShowGroundPoints);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewAerialPoints()
{
    mShowAerialPoints = !mShowAerialPoints;
    //mpUI->SceneWidget->setAerialMeshVisibility(mShowAerialPoints);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewGroundMesh()
{
    mShowGroundMesh = !mShowGroundMesh;
    //mpUI->SceneWidget->setGroundMeshEdgeVisibility(mShowGroundMesh);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewAerialMesh()
{
    mShowAerialMesh = !mShowAerialMesh;
    //mpUI->SceneWidget->setAerialMeshEdgeVisibility(mShowAerialMesh);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewReferencePoint()
{
//    mShowReferencePoint = !mShowReferencePoint;
}


//-----------------------------------------------------------------------------
void cMainWindow::onViewDollyPathSpiderCam()
{
//    bool visible = mpViewDollyPathSpiderCam->isChecked();
//    mpUI->SceneWidget->setSpiderCamDollyPathVisibility(visible);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewDollyPathGPS()
{
//    bool visible = mpViewDollyPathGPS->isChecked();
//    mpUI->SceneWidget->setGpsDollyPathVisibility(visible);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewDollyPathComputed()
{
//    bool visible = mpViewDollyPathComputed->isChecked();
//    mpUI->SceneWidget->setComputedDollyPathVisibility(visible);
}

//-----------------------------------------------------------------------------
void cMainWindow::onViewPointCloudHeight()
{
/*
    auto pPointCloudGenerator = mData.getOusterInfo()->getPointCloudGenerator().lock();

    if (!pPointCloudGenerator->hasPointCloud())
        return;

    cRappPointCloud pointCloud = pPointCloudGenerator->getPointCloud();

    auto* pDlg = new cPointCloudHeightDlg(pointCloud);

    pDlg->exec();

    pDlg->deleteLater();
*/
}

//-----------------------------------------------------------------------------
void cMainWindow::onSettingsLoadGroundMesh()
{
    QString savedFileName = mSettings.value("Defaults/groundMeshFile").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Ground Data..."), savedFileName,
        "GPS CSV Files (*.csv)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Test to make sure the file exists file
    if (!std::filesystem::exists(fileName.toStdString()))
        return;

    LoadGroundReferenceData(fileName);

    mSettings.setValue("Defaults/groundMeshFile", fileName);
}

//-----------------------------------------------------------------------------
void cMainWindow::onSettingsLoadAerialMesh()
{
    QString savedFileName = mSettings.value("Defaults/aerialMeshFile").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Aerial Data..."), savedFileName,
        "GPS CSV Files (*.csv)");

    // Return on Cancel
    if (fileName.isEmpty())
        return;

    // Test to make sure the file exists file
    if (!std::filesystem::exists(fileName.toStdString()))
        return;

    LoadAerialData(fileName);

    mSettings.setValue("Defaults/aerialMeshFile", fileName);
}

//-----------------------------------------------------------------------------
void cMainWindow::openFile(const QString& fileName)
{
    //mpUI->SceneWidget->removeDataSet();

    //// Create reader
    //vtkSmartPointer<vtkDataSetReader> reader = vtkSmartPointer<vtkDataSetReader>::New();
    //reader->SetFileName(fileName.toStdString().c_str());

    //// Read the file
    //reader->Update();

    //// Add data set to 3D view
    //vtkSmartPointer<vtkDataSet> dataSet = reader->GetOutput();
    //if (dataSet != nullptr) 
    //{
    //    mpUI->SceneWidget->addDataSet(reader->GetOutput());
    //}
}

void cMainWindow::readPointcloudData(const QString& fileName)
{
}

void cMainWindow::onGroundDataUpdate()
{
    //mpUI->SceneWidget->removeGroundMesh();

    //auto ground_points = mGroundData.getGroundPoints();

    //vtkDoubleArray* x = vtkDoubleArray::New();
    //vtkDoubleArray* y = vtkDoubleArray::New();
    //vtkDoubleArray* z = vtkDoubleArray::New();

    //for (const auto& ground_point : ground_points)
    //{
    //    x->InsertNextTuple1(ground_point.x_mm);
    //    y->InsertNextTuple1(ground_point.y_mm);
    //    z->InsertNextTuple1(ground_point.z_mm);
    //}

    //vtkNew<vtkTable> table;
    //table->AddColumn(x);
    //table->AddColumn(y);
    //table->AddColumn(z);

    //// Convert to a table view of the data
    //vtkNew<vtkTableToPolyData> tablePoints;
    //tablePoints->SetInputData(table);
    //tablePoints->SetXColumnIndex(0);
    //tablePoints->SetYColumnIndex(1);
    //tablePoints->SetZColumnIndex(2);
    //tablePoints->Update();

    //// Triangulate the grid points.
    //vtkNew<vtkDelaunay2D> delaunay;
    //delaunay->SetInputData(tablePoints->GetOutput());
    //delaunay->Update();
    //vtkSmartPointer<vtkPolyData> mesh = delaunay->GetOutput();

    //mGroundData.clearGroundMesh();

    //auto m = mesh->GetNumberOfCells();
    //vtkIdList* pts = vtkIdList::New();
    //for (vtkIdType i = 0; i < m; ++i)
    //{
    //    mesh->GetCellPoints(i, pts);
    //    auto p1_id = pts->GetId(0);
    //    auto p2_id = pts->GetId(1);
    //    auto p3_id = pts->GetId(2);

    //    double d_mm = (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm);
    //    d_mm += (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm);
    //    d_mm += (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm);
    //    d_mm += (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    mGroundData.addMeshPoint(ground_points[p1_id], ground_points[p2_id], ground_points[p3_id]);
    //}

    //pts->Delete();

    //vtkSmartPointer<vtkPolyData> data = tablePoints->GetOutput();
    //if (data != nullptr)
    //{
    //    mpUI->SceneWidget->addGroundMesh(data);
    //}
}

void cMainWindow::onGroundPointsUpdate()
{
    //mpUI->SceneWidget->removeGroundMesh();

    //auto ground_points = mFieldData.getGroundPoints();

    //vtkDoubleArray* x = vtkDoubleArray::New();
    //vtkDoubleArray* y = vtkDoubleArray::New();
    //vtkDoubleArray* z = vtkDoubleArray::New();

    //for (const auto& ground_point : ground_points)
    //{
    //    x->InsertNextTuple1(ground_point.x_mm);
    //    y->InsertNextTuple1(ground_point.y_mm);
    //    z->InsertNextTuple1(ground_point.z_mm);
    //}

    //vtkNew<vtkTable> table;
    //table->AddColumn(x);
    //table->AddColumn(y);
    //table->AddColumn(z);

    //// Convert to a table view of the data
    //vtkNew<vtkTableToPolyData> tablePoints;
    //tablePoints->SetInputData(table);
    //tablePoints->SetXColumnIndex(0);
    //tablePoints->SetYColumnIndex(1);
    //tablePoints->SetZColumnIndex(2);
    //tablePoints->Update();

    //// Triangulate the grid points.
    //vtkNew<vtkDelaunay2D> delaunay;
    //delaunay->SetInputData(tablePoints->GetOutput());
    //delaunay->Update();
    //vtkSmartPointer<vtkPolyData> mesh = delaunay->GetOutput();

    //mFieldData.clearGroundMesh();

    //auto m = mesh->GetNumberOfCells();
    //vtkIdList* pts = vtkIdList::New();
    //for (vtkIdType i = 0; i < m; ++i)
    //{
    //    mesh->GetCellPoints(i, pts);
    //    auto p1_id = pts->GetId(0);
    //    auto p2_id = pts->GetId(1);
    //    auto p3_id = pts->GetId(2);

    //    double d_mm = (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p2_id].x_mm);
    //    d_mm += (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm) * (ground_points[p1_id].x_mm - ground_points[p3_id].x_mm);
    //    d_mm += (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm) * (ground_points[p1_id].y_mm - ground_points[p3_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm) * (ground_points[p3_id].x_mm - ground_points[p2_id].x_mm);
    //    d_mm += (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm) * (ground_points[p3_id].y_mm - ground_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    mFieldData.addGroundMeshPoint(ground_points[p1_id], ground_points[p2_id], ground_points[p3_id]);
    //}

    //pts->Delete();

    //vtkSmartPointer<vtkPolyData> data = tablePoints->GetOutput();
    //if (data != nullptr)
    //{
    //    mpUI->SceneWidget->addGroundMesh(data);
    //}
}


void cMainWindow::onAerialPointsUpdate()
{
    //mpUI->SceneWidget->removeAerialMesh();

    //auto aerial_points = mFieldData.getAerialPoints();

    //vtkDoubleArray* x = vtkDoubleArray::New();
    //vtkDoubleArray* y = vtkDoubleArray::New();
    //vtkDoubleArray* z = vtkDoubleArray::New();

    //for (const auto& aerial_point : aerial_points)
    //{
    //    x->InsertNextTuple1(aerial_point.x_mm);
    //    y->InsertNextTuple1(aerial_point.y_mm);
    //    z->InsertNextTuple1(aerial_point.z_mm);
    //}

    //vtkNew<vtkTable> table;
    //table->AddColumn(x);
    //table->AddColumn(y);
    //table->AddColumn(z);

    //// Convert to a table view of the data
    //vtkNew<vtkTableToPolyData> tablePoints;
    //tablePoints->SetInputData(table);
    //tablePoints->SetXColumnIndex(0);
    //tablePoints->SetYColumnIndex(1);
    //tablePoints->SetZColumnIndex(2);
    //tablePoints->Update();

    //// Triangulate the grid points.
    //vtkNew<vtkDelaunay2D> delaunay;
    //delaunay->SetInputData(tablePoints->GetOutput());
    //delaunay->Update();
    //vtkSmartPointer<vtkPolyData> mesh = delaunay->GetOutput();

    //mFieldData.clearAerialMesh();

    //auto m = mesh->GetNumberOfCells();
    //vtkIdList* pts = vtkIdList::New();
    //for (vtkIdType i = 0; i < m; ++i)
    //{
    //    mesh->GetCellPoints(i, pts);
    //    auto p1_id = pts->GetId(0);
    //    auto p2_id = pts->GetId(1);
    //    auto p3_id = pts->GetId(2);

    //    double d_mm = (aerial_points[p1_id].x_mm - aerial_points[p2_id].x_mm) * (aerial_points[p1_id].x_mm - aerial_points[p2_id].x_mm);
    //    d_mm += (aerial_points[p1_id].y_mm - aerial_points[p2_id].y_mm) * (aerial_points[p1_id].y_mm - aerial_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (aerial_points[p1_id].x_mm - aerial_points[p3_id].x_mm) * (aerial_points[p1_id].x_mm - aerial_points[p3_id].x_mm);
    //    d_mm += (aerial_points[p1_id].y_mm - aerial_points[p3_id].y_mm) * (aerial_points[p1_id].y_mm - aerial_points[p3_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    d_mm = (aerial_points[p3_id].x_mm - aerial_points[p2_id].x_mm) * (aerial_points[p3_id].x_mm - aerial_points[p2_id].x_mm);
    //    d_mm += (aerial_points[p3_id].y_mm - aerial_points[p2_id].y_mm) * (aerial_points[p3_id].y_mm - aerial_points[p2_id].y_mm);
    //    d_mm = sqrt(d_mm);

    //    if (d_mm > 10000)
    //    {
    //        continue;
    //    }

    //    mFieldData.addAerialMeshPoint(aerial_points[p1_id], aerial_points[p2_id], aerial_points[p3_id]);
    //}

    //pts->Delete();

    //vtkSmartPointer<vtkPolyData> data = tablePoints->GetOutput();
    //if (data != nullptr)
    //{
    //    mpUI->SceneWidget->addAerialMesh(data);
    //}
}

void cMainWindow::onPointCloudUpdate()
{
}

void cMainWindow::onPointCloudComplete()
{
    //auto n = mFieldData.numOfPointCloud() - mNumPointClouds;

    //QProgressDialog progress("Processing imported point clouds...          ", "Abort", 0, 100, this);
    //progress.setWindowModality(Qt::WindowModal);
    //progress.setMinimumDuration(0);
    //progress.setValue(0);

    //update();

    //QAbstractEventDispatcher* pDispatcher = QApplication::eventDispatcher();
    //pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);

#if 0
    std::size_t i = 1;
    for (auto it = mData.pcBegin(); it != mData.pcEnd(); ++it)
    {
        progress.setValue((100.0 * i++)/n);

        auto pc = it->second;

        if (pc == nullptr)
            continue;

        mpUI->SceneWidget->addPointCloud(*pc);
        pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);

/*
        vtkDoubleArray* x = vtkDoubleArray::New();
        vtkDoubleArray* y = vtkDoubleArray::New();
        vtkDoubleArray* z = vtkDoubleArray::New();

        for (const auto& point : pc->data())
        {
            x->InsertNextTuple1(point.x_mm);
            y->InsertNextTuple1(point.y_mm);
            z->InsertNextTuple1(point.z_mm);
        }

        vtkNew<vtkTable> table;
        table->AddColumn(x);
        table->AddColumn(y);
        table->AddColumn(z);

        // Convert to a table view of the data
        vtkNew<vtkTableToPolyData> tablePoints;
        tablePoints->SetInputData(table);
        tablePoints->SetXColumnIndex(0);
        tablePoints->SetYColumnIndex(1);
        tablePoints->SetZColumnIndex(2);
        tablePoints->Update();

        vtkSmartPointer<vtkPolyData> data = tablePoints->GetOutput();
        if (data != nullptr)
        {
            mpUI->SceneWidget->addPointCloud(data);
            pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);
        }
*/
    }
#endif
}

//-----------------------------------------------------------------------------
void cMainWindow::LoadGroundReferenceData(QString fileName)
{
    //// Test to make sure the file exists file
    //if (!std::filesystem::exists(fileName.toStdString()))
    //    return;

    //cGpsFileReader gps;
    //gps.loadFromFile(fileName.toStdString());

    //auto rapp_points = gps.GetRappPoints();

    //mGroundData.addGroundPoints(rapp_points);

    //auto data = mGroundData.getGroundPoints();
    //auto mesh = computeDelaunay2dMesh(data);

    //mGroundData.clearGroundMesh();
    //mGroundData.addMeshData(mesh);

    //onGroundDataUpdate();
}

//-----------------------------------------------------------------------------
void cMainWindow::LoadGroundData(QString fileName)
{
    //// Test to make sure the file exists file
    //if (!std::filesystem::exists(fileName.toStdString()))
    //    return;

    //cGpsFileReader gps;
    //gps.loadFromFile(fileName.toStdString());

    //auto rapp_points = gps.GetRappPoints();

    //mFieldData.addGroundPoints(rapp_points);

    //auto data = mFieldData.getGroundPoints();
    //auto mesh = computeDelaunay2dMesh(data);

    //mFieldData.clearGroundMesh();
    //mFieldData.addGroundMeshData(mesh);
}

//-----------------------------------------------------------------------------
void cMainWindow::LoadAerialData(QString fileName)
{
    //// Test to make sure the file exists file
    //if (!std::filesystem::exists(fileName.toStdString()))
    //    return;

    //cGpsFileReader gps;
    //gps.loadFromFile(fileName.toStdString());

    //auto rapp_points = gps.GetRappPoints();

    //mFieldData.addAerialPoints(rapp_points);

    //auto data = mFieldData.getAerialPoints();
    //auto mesh = computeDelaunay2dMesh(data);

    //mFieldData.clearAerialMesh();
    //mFieldData.addAerialMeshData(mesh);
}


