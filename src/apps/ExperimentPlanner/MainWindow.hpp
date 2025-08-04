
#pragma once

#include "ExperimentFile.hpp"

#include "RappGroundModel.hpp"
#include "RappAerialModel.hpp"

#include <spidercam/spidercam_types.hpp>

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
class cPlannerDataModel;
class cMeasurementManager;
class cMeasurementTreeItem;
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
    void measurementRunning();
    void measurementCompleted();
    void refreshDisplay();
    void defaultMeasurementPathChange(const QString& path);

signals:
    void connectedToController();
    void disconnectedFromController();

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);
    void onLogMessage(uint8_t type, QString device, QString msg);

    void onMeasurementTerminated();
    void onMeasurementCompleted();


// Slots associated with "File" menu actions
private slots:
    void onFileNewMeasurement();
    void onFileOpenMeasurement();
    void onFileSaveMeasurementFile();
    void onFileSaveAsMeasurementFile();
    void onFileSaveAllMeasurementFiles();
    void onFileCloseMeasurementFile();
    void onFileCloseAllMeasurementFiles();

// Slots associated with "Edit" menu actions
private slots:
    void onEditMeasurementMetaInfo();
    void onEditMeasurementCtrlInfo();
    void onEditMeasurementSernsorInfo();
    void onEditAddMeasurementToLayout();
    void onEditMoveMeasurementX();
    void onEditMoveMeasurementY();
    void onEditMoveMeasurementZ();
    void onEditShiftMeasurement();
    void onEditRecomputeHeight();

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

    // Slots associated with "Compute" menu actions
private slots:
    void onComputeSpidercamHeight();
    void onComputeSensorRange();
    void onComputeReferenceHeight();

// Slots associated with "Preference" menu actions
private slots:
    void onPreferenceDefaultMeasurementDirectory();
    void onPreferenceLoadGroundMesh();
    void onPreferenceLoadAerialMesh();
    void onPreferenceDefaultFieldLayoutFile();
    void onPreferenceDefaultPlotSplitDirectory();
    void onPreferenceDefaultFieldBoundaries();

    // Slots associated with "Spidercam" menu actions
private slots:
    void onConnectToSpidercam();
    void onDisconnectFromSpidercam();
    void onSpidercamTestMeasurement();
    void onSpidercamStopMeasurement();
    void onSpidercamPauseRunMeasurement();

// Slots associated with "Help" menu actions
private slots:
    void onHelpAbout();

// Helper slots
private slots:
    void onOpenMeasurement(const QString& filename);
    void onMeasurementChange(QSharedPointer<cExperimentFile> experiment);
    void onMeasurementListUpdateNeeded();

    void onMeasurementRun(const QString& filename);

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
    bool loadMeasurement(const cMeasurementTreeItem& measurement);
    bool loadMeasurement(const std::filesystem::path& measurement);

private:
    void LoadGpsData(QString fileName);
    void LoadAerialData(QString fileName);

private:
    cRappGroundModel mGroundData;
    cRappAerialModel mAerialData;
    spidercam::sWorkingDimensions mLimits;
    int mReferenceHeight_mm = 0;

    QSettings mSettings;

    QMdiArea* mpMdiArea = nullptr;

    cMeasurementManager* mpMeasurements = nullptr;
    cFieldLayoutWidget* mpFieldLayout = nullptr;

    QString mMeasurementFilesPath;
    QString mFieldLayoutFile;
    QString mPlotSplitsPath;

    QMenu* mpFileMenu = nullptr;
    QMenu* mpEditMenu = nullptr;
    QMenu* mpGenerateMenu = nullptr;
    QMenu* mpComputeMenu = nullptr;
    QMenu* mpPreferencesMenu = nullptr;
    QMenu* mpSpidercamMenu = nullptr;
    QMenu* mpViewMenu = nullptr;
    QMenu* mpHelpMenu = nullptr;

    QAction* mpSpidercamConnect = nullptr;
    QAction* mpTestMeasurement = nullptr;
    QAction* mpStopMeasurement = nullptr;
    QAction* mpPauseRunMeasurement = nullptr;

    QToolBar* mpFileBar = nullptr;

    Ui::MainWindow* mpUI = nullptr;
    QString mCurrentFile;

    cPlannerDataModel* mpModel = nullptr;
};

