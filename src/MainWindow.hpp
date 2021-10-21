
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
class QTreeWidget;
class QSplashScreen;
QT_END_NAMESPACE


// Forward Declaration
class GpsWidget;


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

    void initialize(QSplashScreen* pSplashScreen = nullptr);

public slots:
    void onStatusUpdate(QString msg);

private slots:
    void fileNew();
    void fileAddExperiment();
    void helpAbout();

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createStatusBar();
    void createDockWindows();
    bool createExperimentController();
    void createSensorModelsAndViews();

    QSplashScreen* mpSplashScreen;

    QTreeWidget* mpExperiments;

    QMenu* mpFileMenu;
    QMenu* mpViewMenu;
    QMenu* mpHelpMenu;

    Ui::MainWindow* mpUI;
    QString mCurrentFile;

    cDataModel mMainModel;
};

