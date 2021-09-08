
#pragma once

#include "DataModel.hpp"

#include <QMainWindow>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTreeWidget;
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
    void createSensorModelsAndViews();

    QTreeWidget* mpExperiments;

    QMenu* mpFileMenu;
    QMenu* mpViewMenu;
    QMenu* mpHelpMenu;

    Ui::MainWindow* mpUI;
    QString mCurrentFile;

    cDataModel mMainModel;
};

