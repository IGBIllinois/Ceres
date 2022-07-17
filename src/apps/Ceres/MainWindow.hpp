
#pragma once

#include <vector>
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
class cExperimentManager;
class cExperimentTreeItem;

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

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);
    void onLogMessage(uint8_t type, QString device, QString msg);

    void onExperimentCompleted();

private slots:
    void fileRefresh();
    void fileAddExperiment();
    void experimentLoad();
    void experimentRun();
    void experimentPause();
    void experimentStop();
    void helpAbout();

private:
    bool loadExperiment(const cExperimentTreeItem& experiment);

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

    cCeresSplashScreen* mpSplashScreen;

    cExperimentManager* mpExperiments;

    QString mDefaultDataPath;
    QString mExperimentFilesPath;

    QMenu* mpFileMenu;
    QMenu* mpExperimentMenu;

    QAction* mpExpLoad;
    QAction* mpExpRun;
    QAction* mpExpPause;
    QAction* mpExpStop;

    QMenu* mpViewMenu;
    QMenu* mpHelpMenu;

    QToolBar* mpFileBar;

    Ui::MainWindow* mpUI;
    QString mCurrentFile;

    cCtrlDataModel* mpModel;

    cExperimentControlView* mpController;

    std::vector<cExperimentTreeItem*> mBatchProcess;
};

