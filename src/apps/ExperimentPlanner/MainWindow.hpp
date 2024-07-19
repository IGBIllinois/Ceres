
#pragma once

#include "ExperimentFile.hpp"

#include "RappGroundModel.hpp"

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
class QMdiArea;
QT_END_NAMESPACE


// Forward Declarations
class cExperimentManager;
class cExperimentTreeItem;
class cFieldLayoutWidget;
class cExperimentDesignWidget;
class cExperimentDesignMdiChild;


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
    void defaultExperimentPathChange(const QString& path);

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);
    void onLogMessage(uint8_t type, QString device, QString msg);


// Slots associated with "File" menu actions
private slots:
    void onFileNewExperiment();
    void onFileOpenExperiment();
    void onFileSaveExperimentFile();
    void onFileSaveAsExperimentFile();
    void onFileSaveAllExperimentFiles();
    void onFileCloseExperimentFile();
    void onFileCloseAllExperimentFiles();

// Slots associated with "Edit" menu actions
private slots:
    void onEditExperimentMetaInfo();
    void onEditExperimentCtrlInfo();
    void onEditExperimentSernsorInfo();
    void onEditAddExperimentToLayout();
    void onEditMoveExperimentX();
    void onEditMoveExperimentY();
    void onEditMoveExperimentZ();
    void onEditShiftExperiment();

    // Slots associated with "Generate" menu actions
private slots:
    void onGenerateLidarScan_SpiderCam_Point();
    void onGenerateLidarScan_GPS();
    void onGenerateLidarScan_PlotInfo();

    void onGenerateHyperspectralRefScan_SpiderCam_Point();
    void onGenerateHyperspectralRefScan_GPS();

    void onGenerateHyperspectralScan_SpiderCam_Point();
    void onGenerateHyperspectralScan_GPS();
    void onGenerateHyperspectralScan_PlotInfo();

// Slots associated with "Preference" menu actions
private slots:
    void onPreferenceDefaultExperimentDirectory();
    void onPreferenceLoadGroundMesh();
    void onPreferenceDefaultFieldLayoutFile();
    void onPreferenceDefaultPlotSplitDirectory();
    void onPreferenceDefaultFieldBoundaries();

// Slots associated with "Help" menu actions
private slots:
    void onHelpAbout();

// Helper slots
private slots:
    void onOpenExperiment(const QString& filename);
    void onExperimentChange(QSharedPointer<cExperimentFile> experiment);
    void onExperimentListUpdateNeeded();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    cExperimentDesignMdiChild* createMdiChild();

private:
    void LoadGpsData(QString fileName);

private:
    cRappGroundModel mData;

    QSettings mSettings;

    QMdiArea* mpMdiArea = nullptr;

    cExperimentManager* mpExperiments = nullptr;
    cFieldLayoutWidget* mpFieldLayout = nullptr;

    QString mExperimentFilesPath;
    QString mFieldLayoutFile;
    QString mPlotSplitsPath;

    QMenu* mpFileMenu = nullptr;
    QMenu* mpEditMenu = nullptr;
    QMenu* mpGenerateMenu = nullptr;
    QMenu* mpPreferencesMenu = nullptr;
    QMenu* mpViewMenu = nullptr;
    QMenu* mpHelpMenu = nullptr;

    QToolBar* mpFileBar = nullptr;

    Ui::MainWindow* mpUI = nullptr;
    QString mCurrentFile;
};

