
#pragma once

#include <QMainWindow>
#include <QSettings>

//#include "RappFieldDataModel.hpp"
//#include "GroundDataModel.hpp"

#include <vector>
#include <memory>
#include <filesystem>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QToolBar;
QT_END_NAMESPACE


namespace Ui 
{
	class cMainWindow;
}


class cMainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    explicit cMainWindow(QWidget* parent = nullptr);
    ~cMainWindow();

    void initialize();

public slots:
    //! Show the 'About this application' dialog
    void showAboutDialog();

    //! Show the 'Open file...' dialog
    void showOpenFileDialog();

protected slots:
    // Slots associated with file menu actions
    void onFileLoadAerialMesh();
    void onFileLoadGroundMesh();
    void onFileLoadFieldScan();
    void onFileLoadFieldScans();
    void onFileSaveAerialMesh();
    void onFileSaveGroundMesh();
    void onFileExportAerialMeshAsPlyFile();
    void onFileExportGroundMeshAsPlyFile();

protected slots:
    //! Compute Menu Actions
    void onComputeDollyPathSpiderCam();
    void onComputeDollyPathGPS(int device_id);

    void onComputeAerialMesh();
    void onComputeClearAerialMesh();


protected slots:
    //! View Menu Actions
    void onViewGroundPoints();
    void onViewAerialPoints();
    void onViewGroundMesh();
    void onViewAerialMesh();
    void onViewReferencePoint();
    void onViewDollyPathSpiderCam();
    void onViewDollyPathGPS();
    void onViewDollyPathComputed();
    void onViewPointCloudHeight();


protected slots:
    //! Settings Menu Actions
    void onSettingsLoadGroundMesh();
    void onSettingsLoadAerialMesh();

public slots:
    void onStatusUpdate(QString text);
    void onScanLoadComplete();

protected slots:
    void onGroundDataUpdate();
    void onGroundPointsUpdate();
    void onAerialPointsUpdate();
    void onPointCloudUpdate();
    void onPointCloudComplete();

protected:
    //! Open a file
    /*!
        \param[in] fileName The name of the file including the path
     */
    void openFile(const QString& fileName);

    void readPointcloudData(const QString& fileName);

signals:
    void closeProgressDialog();
    void enableSavePointCloud(bool enable);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

private:
    void LoadGroundReferenceData(QString fileName);
    void LoadGroundData(QString fileName);
    void LoadAerialData(QString fileName);

private:
    QSettings mSettings;

    QString             mFileName;
//    cRappFieldDataModel mFieldData;
//    cGroundDataModel    mGroundData;

    QMenu* mpFileMenu = nullptr;
    QMenu* mpComputeMenu = nullptr;
    QMenu* mpViewMenu = nullptr;
    QMenu* mpSettingMenu = nullptr;
    QMenu* mpHelpMenu = nullptr;

    QAction* mpViewDollyPathSpiderCam = nullptr;
    QAction* mpViewDollyPathGPS = nullptr;
    QAction* mpViewDollyPathComputed = nullptr;
    QAction* mpViewAerialMesh = nullptr;

    bool mShowGroundPoints = true;
    bool mShowAerialPoints = true;

    bool mShowGroundMesh = false;
    bool mShowAerialMesh = false;

    Ui::cMainWindow* mpUI;

    std::size_t mNumPointClouds = 0;
    std::vector<std::filesystem::path> mBatchProcess;
};

