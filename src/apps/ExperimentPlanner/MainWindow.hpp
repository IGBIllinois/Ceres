
#pragma once

#include "ExperimentFile.hpp"

#include <vector>
#include <filesystem>

#include <QMainWindow>
#include <QString>
#include <QSettings>

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
class cExperimentManager;
class cExperimentTreeItem;
class cFieldLayoutWidget;
class cExperimentDesignWidget;


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

    void initialize();

signals:
    void refreshDisplay();

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);
    void onLogMessage(uint8_t type, QString device, QString msg);


// Slots associated with "File" menu actions
private slots:
    void onFileNewExperiment_Blank();
    void onFileNewExperiment_GPS();
    void onFileOpenExperiment();
    void onFileSaveExperimentFile();
    void onFileSaveAsExperimentFile();

// Slots associated with "Edit" menu actions
private slots:
    void onEditExperimentMetaInfo();

// Slots associated with "Preference" menu actions
private slots:
    void onPreferenceDefaultExperimentDirectory();
    void onPreferenceDefaultFieldLayoutFile();
    void onPreferenceDefaultPlotSplitDirectory();

// Slots associated with "Help" menu actions
private slots:
    void onHelpAbout();

private:
    bool loadExperiment(const cExperimentTreeItem& experiment);
    bool loadExperiment(const std::filesystem::path& experiment);

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createDataModel(const nlohmann::json& configDoc);

    void doSaveCheck();

private:
    QSettings mSettings;

    cExperimentFile mExperimentFile;

    cExperimentManager* mpExperiments = nullptr;
    cFieldLayoutWidget* mpFieldLayout = nullptr;
    cExperimentDesignWidget* mpExpDesign = nullptr;

    QString mExperimentFilesPath;
    QString mFieldLayoutFile;
    QString mPlotSplitsPath;

    QMenu* mpFileMenu = nullptr;
    QMenu* mpEditMenu = nullptr;
    QMenu* mpPreferencesMenu = nullptr;
    QMenu* mpViewMenu = nullptr;
    QMenu* mpHelpMenu = nullptr;

    QToolBar* mpFileBar = nullptr;

    Ui::MainWindow* mpUI = nullptr;
    QString mCurrentFile;
};

