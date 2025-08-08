
#pragma once

#include <vector>
#include <map>
#include <filesystem>

#include <QMainWindow>
#include <QString>
#include <QSettings>

#include <Sound.hpp>

#include <nlohmann/json.hpp>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QLineEdit;
class QToolBar;
QT_END_NAMESPACE


// Forward Declarations
class cCeresSplashScreen;
class cCtrlDataModel;
class cExperimentControlView;
class cMeasurementManager;
class cMeasurementTreeItem;
class cHobbsMeter;

namespace Ui 
{
    class MainWindow;
}

class cMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cMainWindow(QWidget* parent = nullptr);
    ~cMainWindow();

    void initialize(cCeresSplashScreen* pSplashScreen = nullptr);

signals:
    void experimentRunning();
    void experimentPaused();
    void experimentStopped();
    void refreshDisplay();
    void showMessage(const QString& message, int timeout = 0);
    void setExperimentActions(bool load, bool run, bool pause, bool stop);


public slots:
    void addSensorPropertyPage(QAction* pAction);
    void removeSensorPropertyPage(QAction* pAction);

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);
    void onLogMessage(uint8_t type, QString device, QString instance, QString msg);

//    void onRunControllerAction(QString msg);
    void onExperimentTerminated();
    void onExperimentCompleted();

private slots:
    //! File Menu Actions
    void onFileRefresh();

private slots:
    void onExperimentLoad();
    void onExperimentRun();
    void onExperimentPause();
    void onExperimentStop();

private slots:
    //! Settings Menu Actions
    void onSettingsOptions();
    void onSettingsLoadGroundMesh();
    void onSettingsLoadAerialMesh();

private slots:
    void helpAbout();
    void onSetExperimentActions(bool load, bool run, bool pause, bool stop);

private:
    bool loadMeasurement(const cMeasurementTreeItem& measurement);
    bool loadMeasurement(const std::filesystem::path& measurement);

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWindows(const nlohmann::json& configDoc);
    void createDataModel(const nlohmann::json& configDoc);
    void createExperimentController(const nlohmann::json& configDoc);
    void createSensorModelsAndViews(const nlohmann::json& configDoc);

    void startDataModel(const nlohmann::json& configDoc);

private:
    QSettings mSettings;

    cCeresSplashScreen* mpSplashScreen = nullptr;

    cMeasurementManager* mpMeasurements = nullptr;

    QString mDefaultDataPath;
    QString mExperimentFilesPath;

    QMenu* mpFileMenu = nullptr;
    QMenu* mpExperimentMenu = nullptr;
    QMenu* mpSettingMenu = nullptr;

    QAction* mpExpLoad = nullptr;
    QAction* mpExpRun = nullptr;
    QAction* mpExpPause = nullptr;
    QAction* mpExpStop = nullptr;

    QMenu* mpViewMenu = nullptr;
    QMenu* mpSensorMenu = nullptr;
    QMenu* mpHelpMenu = nullptr;

    QToolBar* mpFileBar = nullptr;

    cHobbsMeter* mpHobbsMeter = nullptr;

    Ui::MainWindow* mpUI = nullptr;
    QString mCurrentFile;

    cCtrlDataModel* mpModel = nullptr;

    cExperimentControlView* mpController = nullptr;

    cSound mExperimentErrorSound;
    cSound mEndOfExperimentSound;

    std::string mBatchFileName;
    std::vector<std::filesystem::path> mBatchProcess;

    std::map<std::string, nlohmann::json> mControllerActions;
};

