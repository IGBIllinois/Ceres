
#pragma once

#include <vector>
#include <map>
#include <filesystem>

#include <QMainWindow>
#include <QString>

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
    void fileRefresh();
    void fileAddExperiment();
    void onExperimentLoad();
    void onExperimentRun();
    void onExperimentPause();
    void onExperimentStop();
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

    cCeresSplashScreen* mpSplashScreen;

    cMeasurementManager* mpMeasurements;

    QString mDefaultDataPath;
    QString mExperimentFilesPath;

    QMenu* mpFileMenu;
    QMenu* mpExperimentMenu;

    QAction* mpExpLoad;
    QAction* mpExpRun;
    QAction* mpExpPause;
    QAction* mpExpStop;

    QMenu* mpViewMenu;
    QMenu* mpSensorMenu;
    QMenu* mpHelpMenu;

    QToolBar* mpFileBar;

    cHobbsMeter* mpHobbsMeter;

    Ui::MainWindow* mpUI;
    QString mCurrentFile;

    cCtrlDataModel* mpModel;

    cExperimentControlView* mpController;

    std::string mBatchFileName;
    std::vector<std::filesystem::path> mBatchProcess;

    std::map<std::string, nlohmann::json> mControllerActions;
};

