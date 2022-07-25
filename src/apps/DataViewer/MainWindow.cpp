
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "CentralWidget.hpp"

#include "SsnxModel_file.hpp"
#include "GPS/SsnxView.hpp"

#include "AxisCommunicationsModel_file.hpp"
#include "RGB/AxisCommunicationsView.hpp"

#include "Lidar/OusterView.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QtDebug>


#include <cassert>
#include <iostream>
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

    delete mpSsnxModel;
    delete mpSsnxView;

    delete mpAxisModel;
    delete mpAxisView;
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

    mpSsnxModel = new cSsnxModel_file();
    mpCentralWidget->attach(mpSsnxModel);

    mpSsnxView = new cSsnxView();
    mpSsnxView->topLevelChanged(true);

    QObject::connect(mpSsnxModel, &cSsnxModel_file::updatePVT, mpSsnxView, &cSsnxView::updatePVT);
    QObject::connect(mpSsnxModel, &cSsnxModel_file::updateUTC, mpSsnxView, &cSsnxView::updateUTC);

    mpSsnxView->show();

    mpAxisModel = new cAxisCommunicationsModel_file();
    mpCentralWidget->attach(mpAxisModel);

    mpAxisView = new cAxisCommunicationsView(nullptr);
    mpAxisView->topLevelChanged(true);

    QObject::connect(mpAxisModel, &cAxisCommunicationsModel_file::onNewImage, mpAxisView, &cAxisCommunicationsView::imageUpdated);

    mpAxisView->show();
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

void cMainWindow::closeEvent(QCloseEvent* event)
{
    mpSsnxView->close();
    mpAxisView->close();

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


