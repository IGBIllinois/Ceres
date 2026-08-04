
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "AboutDialog.hpp"
#include "HobbsMeter.hpp"

#include "CeresSplashScreen.hpp"
#include "CeresOptionsDlg.hpp"
#include "ReferenceHeightDlg.hpp"

#include "CtrlDataModel.hpp"
#include "CtrlDataModelLocal.hpp"
#include "CtrlDataModelRemote.hpp"

#include "MeasurementManager.hpp"
#include "MeasurementTreeItem.hpp"
#include "MeasurementToolbar.hpp"
#include "BatchExpConfirmDlg.hpp"

#include "ExperimentCtrlFactory.hpp"
#include "ExperimentCtrlView.hpp"
#include "ExperimentCtrlModel.hpp"
#include "Spidercam/SpidercamView.hpp"

#include "SensorFactory.hpp"
#include "SensorModel.hpp"
#include "SensorPropertyPage.hpp"

#include "RappFieldModel.hpp"

#include <QtWidgets>
#include <QMessageBox>
#include <QToolBar>
#include <QDialog>


#include <cassert>
#include <sstream>
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
    mpUI(new Ui::MainWindow), 
    mSettings("UIUC", "Ceres")
{
    mpUI->setupUi(this);

    setWindowTitle(tr("Ceres"));

    setUnifiedTitleAndToolBarOnMac(true);

    auto cwd = std::filesystem::current_path();
    auto data_path = cwd / "Data";
    auto exp_path = cwd / "Experiments";
    mDefaultDataPath = QString::fromLatin1(data_path.string().c_str());
    mExperimentFilesPath = QString::fromLatin1(exp_path.string().c_str());

    auto wav_file_name = mSettings.value("Defaults/Options/EndOfExperimentWavFilename").toString();

    if (!wav_file_name.isEmpty())
        mEndOfExperimentSound.open(wav_file_name.toStdString());

    wav_file_name = mSettings.value("Defaults/Options/ExperimentErrorWavFilename").toString();

    if (!wav_file_name.isEmpty())
        mExperimentErrorSound.open(wav_file_name.toStdString());
}

//-----------------------------------------------------------------------------
cMainWindow::~cMainWindow()
{
    mpModel->stopDataThread();

    delete mpUI;
    mpUI = nullptr;
}

//-----------------------------------------------------------------------------
void cMainWindow::initialize(cCeresSplashScreen* pSplashScreen)
{
    mpSplashScreen = pSplashScreen;

    std::string cfgFileName = getCfgFilePath();

    if (cfgFileName.empty())
        exit(EXIT_FAILURE);

    nlohmann::json configDoc;

    std::ifstream in;
    in.open(cfgFileName);

    if (!in.is_open())
    {
        QString msg = "Could not open ";
        msg += cfgFileName.c_str();
        msg += " for reading!";

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
        mb.exec();

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

    if (configDoc.contains("default data folder"))
    {
        auto folders = configDoc["default data folder"];
#ifdef WIN32
        if (folders.contains("windows"))
        {
            mDefaultDataPath = QString::fromLatin1(folders["windows"].get<std::string>().c_str());
        }
#endif
    }

    onStatusUpdate("Initializing menus...");
    createMainMenu();
    createSubMenusAndActions();
    createActions();

    onStatusUpdate("Initializing toolbars...");
    createToolBars();

    onStatusUpdate("Initializing status bar...");
    createStatusBar();

    try
    {
        onStatusUpdate("Initializing dock windows...");
        createDockWindows(configDoc);

        onStatusUpdate("Initializing data model...");
        createDataModel(configDoc);

        onStatusUpdate("Initializing experiment controller...");
        createExperimentController(configDoc);

        onStatusUpdate("Initializing sensors...");
        createSensorModelsAndViews(configDoc);

        onStatusUpdate("Starting data model...");
        startDataModel(configDoc);
    }
    catch (const std::exception& e)
    {
        std::string msg = "Error in ";
        msg += cfgFileName;
        msg += ": ";
        msg += e.what();

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
        mb.exec();

        exit(EXIT_FAILURE);
    }

    QString groundFileName = mSettings.value("Defaults/groundMeshFile").toString();
    if (!groundFileName.isEmpty())
        nRFM::load_ground_data(groundFileName.toStdString());

    QString aerialFileName = mSettings.value("Defaults/aerialMeshFile").toString();
    if (!aerialFileName.isEmpty())
        nRFM::load_aerial_data(aerialFileName.toStdString());

    mpSplashScreen = nullptr;

    statusBar()->addPermanentWidget(mpHobbsMeter);

    QTimer::singleShot(1000, mpModel, &cDataModel::startDataThread);
}

//-----------------------------------------------------------------------------
void cMainWindow::onFileRefresh()
{
    mpMeasurements->refresh();

    emit refreshDisplay();
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentLoad()
{
    if (mpModel->isExperimentRunning())
    {
        QString msg = "A measurement is currently running.  Please wait for the measurement to finish or stop the current measurement.";
        QMessageBox mb(QMessageBox::Critical, "Measurement In Progress", msg);
        mb.exec();

        return;
    }

    mBatchFileName.clear();

    auto experiment_list = mpMeasurements->selectedItems();

    if (experiment_list.size() > 1)
    {
        for (const auto& entry : experiment_list)
        {
            auto* pMeasurement = static_cast<cMeasurementTreeItem*>(entry);

            if (pMeasurement->hasMeasurementDocument())
                mBatchProcess.push_back(pMeasurement->getMeasurementFile());
        }

        if (mBatchProcess.empty())
            return;

        auto measurementFile = mBatchProcess.front();
        mBatchProcess.erase(mBatchProcess.begin());

        mpModel->setBatchMode(mBatchProcess.size() > 0);

        loadMeasurement(measurementFile);

        return;
    }

    auto* pMeasurement = static_cast<cMeasurementTreeItem*>(mpMeasurements->currentItem());

    if (pMeasurement == nullptr)
    {
        return;
    }

    if (pMeasurement->hasMeasurementDocument())
    {
        bool result = loadMeasurement(*pMeasurement);
        return;
    }

    if (0 == pMeasurement->childCount())
    {
        return;
    }

    cBatchExpConfirmDlg* pDlg = new cBatchExpConfirmDlg(this);

    pDlg->initialize(pMeasurement);

    auto result = pDlg->exec();
    if (result == QDialog::Rejected)
        return;

    mBatchProcess = pDlg->getSelectedMeasurements();

    if (mBatchProcess.empty())
        return;

    auto measurementFile = mBatchProcess.front();
    mBatchProcess.erase(mBatchProcess.begin());

    mpModel->setBatchMode(mBatchProcess.size() > 0);

    loadMeasurement(measurementFile);
}

//-----------------------------------------------------------------------------
bool cMainWindow::loadMeasurement(const cMeasurementTreeItem& measurement)
{
    QString msg = "Loading measurement \"";
    msg += measurement.text(0);
    msg += "\" from file ";
    msg += measurement.getFilename();
    onStatusUpdate(msg);

    std::filesystem::path fullFilename = measurement.getFilename().toStdString();
    std::string path = fullFilename.parent_path().string();
    std::string name = fullFilename.filename().replace_extension().string();

    auto expDoc = measurement.getMeasurementDocument();
    if (!mpModel->loadExperiment(path, name, expDoc))
    {
        return false;
    }

    return true;
}

bool cMainWindow::loadMeasurement(const std::filesystem::path& measurement_file)
{
    using namespace nlohmann;

    std::string path = measurement_file.parent_path().string();

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

    if (!mpModel->loadExperiment(path, name, jsonDoc))
    {
        QString msg = "Measurement \"";
        msg += QString::fromStdString(name);
        msg += "\" from file ";
        msg += QString::fromStdString(measurement_file.string());
        msg += " failed to load!";
        onStatusUpdate(msg);
        return false;
    }

    mBatchFileName = name;

    return true;
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentRun()
{
    if (mpModel->isExperimentRunning())
    {
        if (mpModel->isExperimentPaused())
        {
            mpModel->startExperiment();

            emit setExperimentActions(false, false, true, true);

        }
        return;
    }

    if (!mpModel->systemReady())
    {
        return;
    }

    // Reload the experiment each time incase the experiment was tweaked
    if (mBatchFileName.empty())
    {
        onExperimentLoad();
    }

    if (!mpModel->isExperimentLoaded())
    {
        return;
    }

    if (mpModel->experimentRequiresDataFile())
    {
        if (mBatchFileName.empty())
        {
            std::string fileName = mpModel->measurementTitle();
            if (!mpModel->openDataFile(mDefaultDataPath, fileName, false))
            {
                mpModel->terminateExperiment();
                return;
            }
        }
        else
        {
            if (!mpModel->openDataFile(mDefaultDataPath, mBatchFileName, true))
            {
                mpModel->terminateExperiment();
                return;
            }
        }
    }

    mpModel->startExperiment();

    emit setExperimentActions(false, false, true, true);

    emit experimentRunning();
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentPause()
{
    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    mpModel->pauseExperiment();

    emit setExperimentActions(false, true, true, true);

    emit experimentPaused();
}

//-----------------------------------------------------------------------------
void cMainWindow::onExperimentStop()
{
    if (!mpModel->isExperimentRunning())
    {
        return;
    }

    mpModel->terminateExperiment();

//    onExperimentTerminated();
}

void cMainWindow::onSetExperimentActions(bool load, bool run, bool pause, bool stop)
{
    mpExpLoad->setEnabled(load);
    mpExpRun->setEnabled(run);
    mpExpPause->setEnabled(pause);
    mpExpStop->setEnabled(stop);
}

//-----------------------------------------------------------------------------
void cMainWindow::onSettingsOptions()
{
    auto* pDlg = new cCeresOptionsDlg();

    auto end_wav_file_name = mSettings.value("Defaults/Options/EndOfExperimentWavFilename").toString();

    pDlg->setEndOfExperimentWavFilename(end_wav_file_name);

    auto error_wav_file_name = mSettings.value("Defaults/Options/ExperimentErrorWavFilename").toString();

    pDlg->setExperimentErrorWavFilename(error_wav_file_name);

    auto result = pDlg->exec();

    if (result == QDialog::Accepted)
    {
        auto filename = pDlg->endOfExperimentWavFilename();

        if (end_wav_file_name != filename)
        {
            mSettings.setValue("Defaults/Options/EndOfExperimentWavFilename", filename);

            if (mEndOfExperimentSound.is_open())
                close();

            mEndOfExperimentSound.open(filename.toStdString());
        }

        filename = pDlg->experimentErrorWavFilename();

        if (error_wav_file_name != filename)
        {
            mSettings.setValue("Defaults/Options/ExperimentErrorWavFilename", filename);

            if (mExperimentErrorSound.is_open())
                close();

            mExperimentErrorSound.open(filename.toStdString());
        }
    }

    pDlg->deleteLater();

}

void cMainWindow::onSettingsLoadGroundMesh()
{
    QString savedFileName = mSettings.value("Defaults/groundMeshFile").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Ground Data..."), savedFileName,
        "GPS CSV Files (*.csv)");

    if (nRFM::load_ground_data(fileName.toStdString()))
        mSettings.setValue("Defaults/groundMeshFile", fileName);
}

void cMainWindow::onSettingsLoadAerialMesh()
{
    QString savedFileName = mSettings.value("Defaults/aerialMeshFile").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Aerial Data..."), savedFileName,
        "Aerial CSV Files (*.csv)");

    if (nRFM::load_aerial_data(fileName.toStdString()))
        mSettings.setValue("Defaults/aerialMeshFile", fileName);
}

void cMainWindow::onSettingsReferenceHeight()
{
    cReferenceHeightDlg dlg(this);

    dlg.setReferenceHeight_mm(nRFM::reference_height_mm());

    cSpidercamView* pView = dynamic_cast<cSpidercamView*>(mpController);

    if (pView)
    {
        dlg.setPosition(pView->x_mm(), pView->y_mm(), pView->z_mm());
    }

    auto result = dlg.exec();

    if (result == QDialog::Accepted)
    {

    }
}

//-----------------------------------------------------------------------------
void cMainWindow::showAboutDialog()
{
    AboutDialog().exec();
}

void cMainWindow::onStatusUpdate(QString msg)
{
    if (mpSplashScreen)
    {
        mpSplashScreen->showMessage(msg);
        return;
    }

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
    QMessageBox msg_box(QMessageBox::Critical, title, msg, QMessageBox::StandardButton::NoButton, this);
    msg_box.exec();
}

void cMainWindow::onLogMessage(uint8_t type, QString device, QString instance, QString msg)
{
    switch (type)
    {
    case logINFO:
        onInfoMessage(device, msg);
        break;
    case logWARNING:
        onWarningMessage(device, msg);
        break;
    case logERROR:
        onErrorMessage(device, msg);
        break;
    }

    onStatusUpdate(msg);
}

void cMainWindow::onExperimentTerminated()
{
    mBatchProcess.clear();

    mpModel->setBatchMode(false);
    mpModel->clearVariableTables();

    emit setExperimentActions(true, true, false, false);

    emit experimentStopped();

    mBatchFileName.clear();

    if (mExperimentErrorSound.is_open())
        mExperimentErrorSound.play();

    emit showMessage("Measurement stopped!");
}

void cMainWindow::onExperimentCompleted()
{
    mBatchFileName.clear();

    while (!mBatchProcess.empty())
    {
        auto measurementFile = mBatchProcess.front();
        mBatchProcess.erase(mBatchProcess.begin());

        if (loadMeasurement(measurementFile))
        {
            onExperimentRun();
            return;
        }
    }

    mpModel->setBatchMode(false);
    mpModel->clearVariableTables();

    emit setExperimentActions(true, true, false, false);

    emit experimentStopped();

    if (mEndOfExperimentSound.is_open())
        mEndOfExperimentSound.play();

    onStatusUpdate("Measurement completed!");
}

//-----------------------------------------------------------------------------
void cMainWindow::updateControllerConnection(bool connected)
{
    if (mControllerConnected && !connected)
    {
        onStatusUpdate("Connection to experiment controller has been lost!");
    }

    mControllerConnected = connected;
}

//-----------------------------------------------------------------------------
void cMainWindow::loopHeartbeatUpdated()
{
    if (mShowRedHeart)
    {
        mpHeartBeat->setPixmap(*mpRedHeart);
        mShowRedHeart = false;
    }
    else
    {
        mpHeartBeat->setPixmap(*mpEmptyHeart);
        mShowRedHeart = true;
    }
}

//-----------------------------------------------------------------------------
void cMainWindow::loopTerminated()
{
    mpHeartBeat->setPixmap(*mpBlackHeart);
}

//-----------------------------------------------------------------------------
void cMainWindow::createMainMenu()
{
    mpFileMenu = mpUI->menuBar->addMenu(tr("&File"));
    mpExperimentMenu = mpUI->menuBar->addMenu(tr("&Experiment"));
    mpViewMenu = mpUI->menuBar->addMenu(tr("&View"));
    mpSensorMenu = mpUI->menuBar->addMenu(tr("&Sensors"));
    mpSettingMenu = mpUI->menuBar->addMenu(tr("Setting"));
    mpHelpMenu = mpUI->menuBar->addMenu(tr("&Help"));
}

//-----------------------------------------------------------------------------
void cMainWindow::createSubMenusAndActions()
{
    QAction* pMenuItem = nullptr;

    // Build the File Menu
    pMenuItem = new QAction(tr("Refresh"), this);
    pMenuItem->setStatusTip(tr("Refresh the experiment window"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onFileRefresh);
    mpFileMenu->addAction(pMenuItem);

    mpFileMenu->addSeparator();

    pMenuItem = new QAction(tr("E&xit"), this);
    pMenuItem->setShortcuts(QKeySequence::Quit);
    pMenuItem->setStatusTip(tr("Exit program"));
    connect(pMenuItem, &QAction::triggered, &QApplication::closeAllWindows);
    mpFileMenu->addAction(pMenuItem);

    // Build the Experiment Menu
    mpExpLoad = new QAction(tr("&Load"), this);
    mpExpLoad->setStatusTip(tr("Load experiment..."));
    connect(mpExpLoad, &QAction::triggered, this, &cMainWindow::onExperimentLoad);
    mpExperimentMenu->addAction(mpExpLoad);

    mpExperimentMenu->addSeparator();

    mpExpRun = new QAction(tr("&Run"), this);
    mpExpRun->setStatusTip(tr("Run experiment..."));
    connect(mpExpRun, &QAction::triggered, this, &cMainWindow::onExperimentRun);
    mpExperimentMenu->addAction(mpExpRun);

    mpExpPause = new QAction(tr("&Pause"), this);
    mpExpPause->setStatusTip(tr("Pause the currently running experiment"));
    connect(mpExpPause, &QAction::triggered, this, &cMainWindow::onExperimentPause);
    mpExperimentMenu->addAction(mpExpPause);
    mpExpPause->setEnabled(false);

    mpExpStop = new QAction(tr("&Stop"), this);
    mpExpStop->setStatusTip(tr("Stop the currently running experiment"));
    connect(mpExpStop, &QAction::triggered, this, &cMainWindow::onExperimentStop);
    mpExperimentMenu->addAction(mpExpStop);
    mpExpStop->setEnabled(false);

    connect(this, &cMainWindow::setExperimentActions, this, &cMainWindow::onSetExperimentActions);

    // Build the View Menu
    /* The view menu is built by the dock window system */

    // Build the Sensor Menu
    /* The sensor menu is built by the sensor loading system */

    // Build the Settings Menu
    pMenuItem = new QAction(tr("Options"), this);
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onSettingsOptions);
    mpSettingMenu->addAction(pMenuItem);

    mpSettingMenu->addSeparator();

    pMenuItem = new QAction(tr("Load Ground Data"), this);
    pMenuItem->setStatusTip(tr("Load the GPS based ground data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onSettingsLoadGroundMesh);
    mpSettingMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Load Aerial Data"), this);
    pMenuItem->setStatusTip(tr("Load the GPS based aerial data"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onSettingsLoadAerialMesh);
    mpSettingMenu->addAction(pMenuItem);

    mpSettingMenu->addSeparator();

    pMenuItem = new QAction(tr("Set Reference Height"), this);
    pMenuItem->setStatusTip(tr("Set the reference height using the geometric height at the current location"));
    connect(pMenuItem, &QAction::triggered, this, &cMainWindow::onSettingsReferenceHeight);
    mpSettingMenu->addAction(pMenuItem);

    // Build the Help Menu
    pMenuItem = new QAction(tr("&About"), this);
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
//    mpFileBar = addToolBar("File");

    auto* toolbar = new cMeasurementToolbar(this);
    connect(this, &cMainWindow::experimentRunning, toolbar, &cMeasurementToolbar::measurementRunning);
    connect(this, &cMainWindow::experimentPaused, toolbar, &cMeasurementToolbar::measurementPaused);
    connect(this, &cMainWindow::experimentStopped, toolbar, &cMeasurementToolbar::measurementStopped);

    connect(toolbar, &cMeasurementToolbar::loadSelected, this, &cMainWindow::onExperimentLoad);
    connect(toolbar, &cMeasurementToolbar::runSelected, this, &cMainWindow::onExperimentRun);
    connect(toolbar, &cMeasurementToolbar::pauseSelected, this, &cMainWindow::onExperimentPause);
    connect(toolbar, &cMeasurementToolbar::stopSelected, this, &cMainWindow::onExperimentStop);

    addToolBar(toolbar);
}

//-----------------------------------------------------------------------------
void cMainWindow::createStatusBar()
{
    statusBar();

    connect(this, &cMainWindow::showMessage, statusBar(), &QStatusBar::showMessage);

    QFontMetrics fm(font());
    int pixelsHeight = fm.boundingRect(" XXX.XXX ").height();

    QPixmap pixmap;
    if (pixmap.load(":/ripe.illinois.edu/heart_red.png"))
    {
        mpRedHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
    }
    else
        mpRedHeart = new QPixmap(":/ripe.illinois.edu/heart_red.png");

    if (pixmap.load(":/ripe.illinois.edu/heart_empty.png"))
    {
        mpEmptyHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
    }
    else
        mpEmptyHeart = new QPixmap(":/ripe.illinois.edu/heart_empty.png");

    if (pixmap.load(":/ripe.illinois.edu/heart_black.png"))
    {
        mpBlackHeart = new QPixmap(pixmap.scaledToHeight(pixelsHeight));
    }
    else
        mpBlackHeart = new QPixmap(":/ripe.illinois.edu/heart_black.png");

    mpHeartBeat = new QLabel(this);
    mpHeartBeat->setPixmap(*mpBlackHeart);
    mShowRedHeart = true;

    statusBar()->addPermanentWidget(mpHeartBeat);

    mpHobbsMeter = new cHobbsMeter(statusBar());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDockWindows(const nlohmann::json& configDoc)
{
    if (configDoc.contains("experiment file folder"))
    {
        auto folders = configDoc["experiment file folder"];
#ifdef WIN32
        if (folders.contains("windows"))
        {
            mExperimentFilesPath = QString::fromLatin1(folders["windows"].get<std::string>().c_str());
        }
#endif
    }

    QDockWidget* dock = new QDockWidget(tr("Measurements"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    mpMeasurements = new cMeasurementManager(mExperimentFilesPath, dock);
    connect(mpMeasurements, &cMeasurementManager::runExperiments, this, &cMainWindow::onExperimentRun);


    dock->setWidget(mpMeasurements);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    mpViewMenu->addAction(dock->toggleViewAction());
}

//-----------------------------------------------------------------------------
void cMainWindow::createDataModel(const nlohmann::json& configDoc)
{
    auto data_model = configDoc["data model"];

    if (data_model.is_string())
    {
        std::string type = data_model.get<std::string>();

        if (type == "local")
        {
            auto* pModel = new cCtrlDataModelLocal(this);

            QObject::connect(&(pModel->getThread()), &cCtrlDataThread::updateLoopHeartbeat, this, &cMainWindow::loopHeartbeatUpdated);
            QObject::connect(&(pModel->getThread()), &cCtrlDataThread::terminated, this, &cMainWindow::loopTerminated);

            mpModel = pModel;
        }
        else
        {
            QString msg = "Unknown data model: ";
            msg += type.c_str();

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", msg);
            mb.exec();

            exit(EXIT_FAILURE);
        }

        QObject::connect(mpModel, &cCtrlDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
        QObject::connect(mpModel, &cCtrlDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
        QObject::connect(mpModel, &cCtrlDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
        QObject::connect(mpModel, &cCtrlDataModel::errorMessage, this, &cMainWindow::onErrorMessage);
    }
    else if (data_model.is_object())
    {
//        std::string c4_ip = data_model["c4_ip"];
//        uint16_t port = data_model["port"];

//        std::string c3_ip;
//        if (data_model.contains("c3_ip"))
//            c3_ip = data_model["c3_ip"];

        cCtrlDataModelRemote* pModel = new cCtrlDataModelRemote(this);

        QObject::connect(pModel, &cCtrlDataModelRemote::updateLoopHeartbeat, this, &cMainWindow::loopHeartbeatUpdated);
        QObject::connect(pModel, &cCtrlDataModelRemote::loopTerminated, this, &cMainWindow::loopTerminated);

        QObject::connect(pModel, &cDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);
        QObject::connect(pModel, &cDataModel::infoMessage, this, &cMainWindow::onInfoMessage);
        QObject::connect(pModel, &cDataModel::warningMessage, this, &cMainWindow::onWarningMessage);
        QObject::connect(pModel, &cDataModel::errorMessage, this, &cMainWindow::onErrorMessage);

        auto* dockWidget = new QDockWidget();
        pModel->createView(dockWidget);

        if (dockWidget->widget() != nullptr)
        {
            dockWidget->setParent(this);

            addDockWidget(Qt::RightDockWidgetArea, dockWidget);
            mpViewMenu->addAction(dockWidget->toggleViewAction());
        }

//        bool result = pModel->try_to_connect(QString(c4_ip.c_str()), port,
//            false, QString(c3_ip.c_str()));

//        if (!result)
//        { }

        mpModel = pModel;
    }

//    QObject::connect(mpModel, &cCtrlDataModel::statusMessage, this, &cMainWindow::onStatusUpdate);

    QObject::connect(mpModel, &cCtrlDataModel::connectToSensorMenu, this, &cMainWindow::addSensorPropertyPage);
    QObject::connect(mpModel, &cCtrlDataModel::disconnectFromSensorMenu, this, &cMainWindow::removeSensorPropertyPage);

    QObject::connect(mpModel, &cCtrlDataModel::experimentTerminated, this, &cMainWindow::onExperimentTerminated);
    QObject::connect(mpModel, &cCtrlDataModel::experimentCompleted, this, &cMainWindow::onExperimentCompleted);
}


//-----------------------------------------------------------------------------
void cMainWindow::startDataModel(const nlohmann::json& configDoc)
{
    auto data_model = configDoc["data model"];

    if (data_model.is_object())
    {
        std::string c4_ip = data_model["c4_ip"];
        uint16_t port = data_model["port"];

        std::string c3_ip;
        if (data_model.contains("c3_ip"))
            c3_ip = data_model["c3_ip"];

        cCtrlDataModelRemote* pModel = dynamic_cast<cCtrlDataModelRemote*>(mpModel);

        if (!pModel)
            return;

        bool result = pModel->try_to_connect(QString(c4_ip.c_str()), port,
            false, QString(c3_ip.c_str()));

        if (!result)
        {
        }
    }
}

//-----------------------------------------------------------------------------
void cMainWindow::createExperimentController(const nlohmann::json& configDoc)
{
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

    sExperimentControllerWidgets widgets = create_experiment_controller(name, ctrlDoc);

    cExperimentControlModel* pModel = widgets.pModel;
    mpController = widgets.pView;

    if ((pModel == nullptr) || (mpController == nullptr))
    {
        std::string msg = "Unknown experiment controller or controller not found: name is \"";
        msg += name;
        msg += "\".";

        QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
        mb.exec();

        exit(EXIT_FAILURE);
    }

    QObject::connect(pModel, &cExperimentControlModel::updateControllerConnection,
            this, &cMainWindow::updateControllerConnection);

    QObject::connect(pModel, &cExperimentControlModel::updateControllerConnection,
        mpHobbsMeter, &cHobbsMeter::updateControllerConnection);

    QObject::connect(pModel, &cExperimentControlModel::experimentStateChanged,
        mpHobbsMeter, &cHobbsMeter::onExperimentStateChange);

    QObject::connect(pModel, &cExperimentControlModel::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(pModel, &cExperimentControlModel::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(pModel, &cExperimentControlModel::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(pModel, &cExperimentControlModel::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::connect(mpController, &cExperimentControlView::statusMessage, this, &cMainWindow::onStatusUpdate);
    QObject::connect(mpController, &cExperimentControlView::infoMessage, this, &cMainWindow::onInfoMessage);
    QObject::connect(mpController, &cExperimentControlView::warningMessage, this, &cMainWindow::onWarningMessage);
    QObject::connect(mpController, &cExperimentControlView::errorMessage, this, &cMainWindow::onErrorMessage);

    QObject::connect(this, &cMainWindow::refreshDisplay, mpController, &cExperimentControlView::refresh);

    QObject::connect(pModel, &cExperimentControlModel::experimentStatus,
        mpController, &cExperimentControlView::experimentStatusUpdating);

    QObject::connect(pModel, &cExperimentControlModel::experimentStateChanged,
        mpController, &cExperimentControlView::experimentStateChanging);

    if (configDoc.contains(name))
    {
        pModel->configure(configDoc[name]);
        mpController->configure(configDoc[name]);
    }

    mpModel->addExperimentControlModel(pModel);

    setCentralWidget(mpController);

    if (widgets.pDockableView)
    {
        widgets.pDockableView->setParent(this);
        widgets.pDockableView->setAllowedAreas(Qt::AllDockWidgetAreas);
        widgets.pDockableView->hide();

        addDockWidget(Qt::BottomDockWidgetArea, widgets.pDockableView);
        mpViewMenu->addAction(widgets.pDockableView->toggleViewAction());
    }

    if (widgets.pStatusBar)
    {
        statusBar()->addPermanentWidget(widgets.pStatusBar);
    }

    if (widgets.pToolBar)
    {
        addToolBar(widgets.pToolBar);
    }
}

//-----------------------------------------------------------------------------
void cMainWindow::createSensorModelsAndViews(const nlohmann::json& configDoc)
{
    auto sensors = configDoc["sensors"];

    for (auto sensor : sensors)
    {
        std::string type = sensor["type"];
        auto widgets = create_sensor(type, sensor);

        if (widgets.pModel == nullptr)
        {
            std::string msg = "Unknown sensor type or sensor not found: type is \"";
            msg += type;
            msg += "\"";

            if (sensor.contains("sensor"))
            {
                msg += ", sensor name: ";
                msg += sensor["sensor"];
            }
            else
                msg += ".";

            QMessageBox mb(QMessageBox::Critical, "Configuration Error", QString(msg.c_str()));
            mb.exec();
            continue;
        }

        QObject::connect(widgets.pModel, &cSensorModel::statusMessage, this, &cMainWindow::onStatusUpdate);
//        QObject::connect(widgets.pModel, &cSensorModel::infoMessage, this, &cMainWindow::onInfoMessage);
//        QObject::connect(widgets.pModel, &cSensorModel::warningMessage, this, &cMainWindow::onWarningMessage);
//        QObject::connect(widgets.pModel, &cSensorModel::errorMessage, this, &cMainWindow::onErrorMessage);
        QObject::connect(widgets.pModel, &cSensorModel::elogMessage, this, &cMainWindow::onLogMessage);

        if (!configDoc.contains(type))
        {
            QString msg = "The follow object is missing from the configuration file: ";
            msg += QString::fromStdString(type);
            onErrorMessage("Configuration Error", msg);
            continue;
        }

        bool validSensor = false;
        try
        {
            std::string entry = type;
            if (sensor.contains("instance"))
            {
                entry += ":";
                entry += sensor["instance"];
            }

            validSensor = widgets.pModel->configure(configDoc[entry]);
        }
        catch (const std::exception& e)
        {
            QString msg = "Sensor: ";
            msg += QString::fromStdString(type);
            msg += "\n";
            msg += e.what();
            onErrorMessage("Configuration Error", msg);
            validSensor = false;
        }

        if (!validSensor)
        {
            remove_sensor(type, widgets);
            continue;
        }

        mpModel->addSensor(widgets.pModel);

        if (widgets.pDockableView)
        {
            widgets.pDockableView->setParent(this);

            addDockWidget(Qt::RightDockWidgetArea, widgets.pDockableView);
            mpViewMenu->addAction(widgets.pDockableView->toggleViewAction());
        }

        if (widgets.pStatusBar)
        {
            statusBar()->addPermanentWidget(widgets.pStatusBar);
        }

        if (widgets.pToolBar)
        {
            addToolBar(widgets.pToolBar);
        }

        if (widgets.pPropertyPage)
        {
            widgets.pPropertyPage->createWidgets();
            widgets.pPropertyPage->doLayout();

            mpModel->addSensorPropertyPage(widgets.pPropertyPage);
        }

        widgets.pModel->updateViews();
    }
}

void cMainWindow::addSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->addAction(pAction);
}

void cMainWindow::removeSensorPropertyPage(QAction* pAction)
{
    mpSensorMenu->removeAction(pAction);
}
