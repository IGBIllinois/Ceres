/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams.
 */

#pragma once

#include "../../SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"


 // Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QComboBox;
class QGroupBox;
class QPushButton;
QT_END_NAMESPACE


class cAxisCommunicationsPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
    Q_OBJECT

public:
    cAxisCommunicationsPropertyPage(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage() = default;

    void createWidgets() override;

    void enableControls(bool enable) override;
    
    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected slots:
    void modeTextChanged(const QString& text);
    virtual void onGrabImagePressed() = 0;

protected:
    QLabel* mpModeLabel = nullptr;
    QComboBox* mpMode = nullptr;

    QLabel* mpCameraIdLabel = nullptr;
    QLineEdit* mpCameraId = nullptr;

    QLabel* mpImageSizeLabel = nullptr;
    QComboBox* mpImageSizes = nullptr;

    QLabel* mpFrameRateLabel = nullptr;
    QLineEdit* mpFrameRate_fps = nullptr;

    QLabel* mpLapseIntervalLabel = nullptr;
    QLineEdit* mpLapseInterval_s = nullptr;

    int mDefaultMode = -1;

    int mDefaultCameraId = -1;

    int  mDefaultFrameRate_fps = -1;
    int  mDefaultLapseInterval_ms = -1;

    int mDefaultImageWidth = -1;
    int mDefaultImageHeight = -1;

    QPushButton* mpGrabImage = nullptr;
};


