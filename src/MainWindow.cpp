
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "SensorFactory.hpp"
#include "GpsModelSsnx.hpp"
#include "GpsWidget.hpp"
//#include "OusterWidget.hpp"

#include <QtWidgets>
#include <QMessageBox>


#include <cassert>
#include <iostream>
#include <sstream>

#include <nlohmann/json.hpp>


//-----------------------------------------------------------------------------
cMainWindow::cMainWindow(QWidget* parent) :
    QMainWindow(parent),
    mpUI(new Ui::MainWindow)
{
    mpUI->setupUi(this);

    createMainMenu();
    createSubMenusAndActions();
    createActions();
    createStatusBar();
    createDockWindows();
    createSensorModelsAndViews();

    setWindowTitle(tr("Ceres"));

    setUnifiedTitleAndToolBarOnMac(true);

    mMainModel.startDataCollection();
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    mMainModel.stopDataCollection();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::fileNew()
{

}

//-----------------------------------------------------------------------------
void cMainWindow::fileAddExperiment()
{
    QTreeWidgetItem* experiment = new QTreeWidgetItem();
    experiment->setText(0, "Hello");
    QTreeWidgetItem* title = new QTreeWidgetItem();
    title->setText(0, "Title");
    experiment->addChild(title);
    mpExperiments->addTopLevelItem(experiment);
}

//-----------------------------------------------------------------------------
void cMainWindow::helpAbout()
{

}

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Menu
    pMenuItem = new QAction(tr("&New"), this);
    pMenuItem->setShortcuts(QKeySequence::New);
    pMenuItem->setStatusTip(tr("Create a new file"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::fileNew);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("&Add Experiment"), this);
    pMenuItem->setShortcuts(QKeySequence::New);
    pMenuItem->setStatusTip(tr("Add an experiment to the list"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::fileAddExperiment);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);

    // Build the View Menu

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
void cMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows()
{
    QDockWidget* dock = new QDockWidget(tr("Experiments"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpExperiments = new QTreeWidget(dock);
    mpExperiments->clear();
    mpExperiments->setColumnCount(1);
    mpExperiments->setHeaderLabel("Loaded Experiments");

    dock->setWidget(mpExperiments);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
void cMainWindow::createSensorModelsAndViews()
{
    std::string cfgFileName = "C:/igb/Ceres/build/bin/Debug/ceres.json";
//    QApplication::arguments();
    QString s = QApplication::applicationDirPath();
    QDockWidget* dockWidget = nullptr;

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        // Note: we can't reuse the wxThreadEvent object
        std::string msg = "Could not open ";
        msg += cfgFileName;
        msg += " for reading!";

        throw std::runtime_error(msg);
    }

    try
    {
        nlohmann::json jsonDoc;
        in >> jsonDoc;

        auto sensors = jsonDoc["sensors"];

        for (std::string name : sensors)
        {
            auto sensor_view = create_sensor(name, this);
            
            cSensorModel* pModel = sensor_view.first;
            QDockWidget* pView = sensor_view.second;

            if ((pModel == nullptr) || (pView == nullptr))
                continue;

            mMainModel.addSensor(pModel);

            if (jsonDoc.contains(name))
            {
                pModel->configure(jsonDoc[name]);
            }

            addDockWidget(Qt::RightDockWidgetArea, pView);
            mpViewMenu->addAction(pView->toggleViewAction());
        }
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error in ";
        msg += cfgFileName;
        msg += ": ";
        msg += e.what();

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
        mb.exec();

        throw std::runtime_error(msg);
    }


    // Create the SSNX model and view...
/*
    auto* gpsModel = new cGpsModelSsnx(this);
//    gpsModel->setParent(this);
    mMainModel.addSensor(gpsModel);

    dockWidget = new QDockWidget(tr("GPS"), this);
    auto* gpsView = new GpsWidget(dockWidget);
    dockWidget->setWidget(gpsView);
    connect(dockWidget, &QDockWidget::dockLocationChanged, gpsView, &cGpsView::dockLocationChanged);
    connect(dockWidget, &QDockWidget::topLevelChanged, gpsView, &cGpsView::topLevelChanged);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    mpViewMenu->addAction(dockWidget->toggleViewAction());

    connect(gpsModel, &cGpsModelSsnx::updatePVT, gpsView, &cGpsView::updatePVT);
    connect(gpsModel, &cGpsModelSsnx::updateUTC, gpsView, &cGpsView::updateUTC);
*/
}
