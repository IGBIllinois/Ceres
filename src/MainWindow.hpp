
#pragma once

#include "DataModel.hpp"

#include <QMainWindow>
#include <QString>


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
class cExperimentControlView;
class cExperimentManager;


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

    void onExperimentCompleted();

private slots:
    void fileNew();
    void fileAddExperiment();
    void experimentLoad();
    void experimentRun();
    void experimentPause();
    void experimentStop();
    void helpAbout();

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    bool createExperimentController();
    void createSensorModelsAndViews();

    cCeresSplashScreen* mpSplashScreen;

    cExperimentManager* mpExperiments;

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

    cDataModel mMainModel;

    cExperimentControlView* mpController;
};

