
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

class cRemoteClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cRemoteClientWindow(QWidget* parent = nullptr);
    ~cRemoteClientWindow();

    void initialize(cCeresSplashScreen* pSplashScreen = nullptr);

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);

private slots:

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createSensorModelsAndViews();

    cCeresSplashScreen* mpSplashScreen;

    QMenu* mpFileMenu;

    QMenu* mpHelpMenu;

    Ui::MainWindow* mpUI;
    QString mCurrentFile;

    cDataModel* mpMainModel;
};

