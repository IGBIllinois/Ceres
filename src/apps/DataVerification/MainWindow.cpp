
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "CentralWidget.hpp"

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

    setWindowTitle(tr("Ceres Data Verification"));

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


