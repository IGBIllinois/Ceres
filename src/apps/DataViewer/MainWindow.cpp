
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "CentralWidget.hpp"

#include "SsnxModel_file.hpp"
#include "GPS/SsnxView.hpp"

#include "AxisCommunicationsModel_file.hpp"
#include "AxisCommunicationsView_file.hpp"

#include "OusterModel_file.hpp"
#include "Lidar/OusterView.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QtDebug>


#include <cassert>
#include <sstream>
#include <chrono>


//-----------------------------------------------------------------------------
cMainWindow::cMainWindow(QWidget* parent) :
    QMainWindow(parent),
    mpFileMenu(nullptr),
    mpUI(new Ui::MainWindow)
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres Data Viewer"));

    setUnifiedTitleAndToolBarOnMac(true);
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

    onStatusUpdate("Initializing status bar...");
    createStatusBar();

    mpCentralWidget = new cCentralWidget(this);
    mpCentralWidget->setEnabled(true);
    connect(mpCentralWidget, &cCentralWidget::statusMessage, this, &cMainWindow::onStatusUpdate);
    connect(mpCentralWidget, &cCentralWidget::errorMessage, this, &cMainWindow::onErrorMessage);

    setCentralWidget(mpCentralWidget);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    onStatusUpdate("");

    mpSsnxModel = new cSsnxModel_file(this);
    mpCentralWidget->attach(mpSsnxModel);

    mpSsnxView = new cSsnxView(this);
    mpSsnxView->topLevelChanged(true);
    mpSsnxView->setWindowFlag(Qt::Tool);

    QObject::connect(mpSsnxModel, &cSsnxModel_file::updatePVT, mpSsnxView, &cSsnxView::updatePVT);
    QObject::connect(mpSsnxModel, &cSsnxModel_file::updateUTC, mpSsnxView, &cSsnxView::updateUTC);

    mpAxisModel = new cAxisCommunicationsModel_file(this);
    mpCentralWidget->attach(mpAxisModel);

    mpAxisView = new cAxisCommunicationsView_file(this);
    mpAxisView->initialize();
    mpAxisView->topLevelChanged(true);
    mpAxisView->setWindowFlag(Qt::Tool);

    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::updateActiveCameraId, 
                    mpAxisView, &cAxisCommunicationsView_file::activeCameraIdUpdated);

    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::updateFramesRate, 
                    mpAxisView, &cAxisCommunicationsView_file::framesRateUpdated);

    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::updateImageSize,
                    mpAxisView, &cAxisCommunicationsView_file::imageSizeUpdated);

    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::onNewImage,
            mpAxisView, &cAxisCommunicationsView::imageUpdated);

    mpOusterModel = new cOusterModel_file(this);
    mpCentralWidget->attach(mpOusterModel);

    mpOusterView = new cOusterView(mpOusterModel, this);
    mpOusterView->topLevelChanged(true);
    mpOusterView->setWindowFlag(Qt::Tool);

    QObject::connect(mpOusterModel, &cOusterModel::updateBeamIntrinsics, mpOusterView, &cOusterView::beamIntrinsicsChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateImuIntrinsics, mpOusterView, &cOusterView::imuIntrinsicsChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateLidarIntrinsics, mpOusterView, &cOusterView::lidarIntrinsicsChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateDataFormat, mpOusterView, &cOusterView::dataFormatChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateAzimuthWindow, mpOusterView, &cOusterView::azimuthWindowChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateImuData, mpOusterView, &cOusterView::imuDataChanged);
    QObject::connect(mpOusterModel, &cOusterModel::updateLidarData, mpOusterView, &cOusterView::displayData);
    //    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::onNewImage, mpAxisView, &cAxisCommunicationsView::imageUpdated);
}

//-----------------------------------------------------------------------------
void cMainWindow::onStatusUpdate(QString msg)
{
    if (statusBar())
        statusBar()->showMessage(msg);
}

//-----------------------------------------------------------------------------
void cMainWindow::onInfoMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Information, title, msg);
    msg_box.exec();
}

//-----------------------------------------------------------------------------
void cMainWindow::onWarningMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Warning, title, msg);
    msg_box.exec();
}

//-----------------------------------------------------------------------------
void cMainWindow::onErrorMessage(QString title, QString msg)
{
    QMessageBox msg_box(QMessageBox::Critical, title, msg);
    msg_box.exec();
}

void cMainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (event->type() == QEvent::Show)
    {
        QSize frame_size = frameSize();
        auto p = pos();
        mpSsnxView->show();
        mpSsnxView->activateWindow();
        mpAxisView->show();
        mpAxisView->activateWindow();
        mpOusterView->show();
        mpOusterView->activateWindow();

        activateWindow();
    }
}

void cMainWindow::closeEvent(QCloseEvent* event)
{
//    mpSsnxView->close();
//    mpAxisView->close();

    QWidget::closeEvent(event);
}

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);
}

//-----------------------------------------------------------------------------
void cMainWindow::createActions()
{

}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();
}


