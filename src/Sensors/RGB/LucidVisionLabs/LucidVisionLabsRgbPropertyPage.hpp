/*
 * The LucidVisionLabsRgbPropertyPage to control the Axis Communications WebCams.
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
class QCheckBox;
class QGroupBox;
class QPushButton;
QT_END_NAMESPACE


class cLucidVisionLabsRgbPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
    Q_OBJECT

public:
    cLucidVisionLabsRgbPropertyPage(QWidget* parent = nullptr);
    ~cLucidVisionLabsRgbPropertyPage() = default;

    void createWidgets() override;

    void enableControls(bool enable) override;
    
    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected slots:
    void modeTextChanged(const QString& text);
    virtual void exposureTimeSelectorChanged(const QString&) = 0;
    virtual void exposureAutoModeChanged(const QString&) = 0;
    virtual void pixelFormatChanged(const QString&) = 0;
    virtual void gainAutoModeChanged(const QString&) = 0;
    virtual void balanceWhiteAutoModeChanged(const QString&) = 0;

    virtual void onGrabImagePressed() = 0;

protected:
    QLabel*    mpModeLabel = nullptr;
    QComboBox* mpMode = nullptr;

    QLabel*    mpFrameRateLabel = nullptr;
    QLineEdit* mpFrameRate_fps = nullptr;

    QLabel*    mpFrameIntervalLabel = nullptr;
    QLineEdit* mpFrameInterval_s = nullptr;

    QGroupBox* mpExposureInfo = nullptr;
    QLineEdit* mpAcqTime_Hz = nullptr;
    QComboBox* mpExposureTimeSelector = nullptr;
    QLineEdit* mpExposureTime_us = nullptr;
    QComboBox* mpExposureAutoMode = nullptr;

    QGroupBox* mpPixelFormatInfo = nullptr;
    QComboBox* mpPixelFormat = nullptr;
    QLineEdit* mpWidth = nullptr;
    QLineEdit* mpHeight = nullptr;
    QLineEdit* mpOffsetX = nullptr;
    QLineEdit* mpOffsetY = nullptr;

    QGroupBox* mpAnalogCtrlInfo = nullptr;
    QLineEdit* mpGain_dB = nullptr;
    QComboBox* mpGainAutoMode = nullptr;
    QComboBox* mpBalanceWhiteAutoMode = nullptr;
    QCheckBox* mpGammaEnable = nullptr;
    QLineEdit* mpGamma = nullptr;

    QPushButton* mpGrabImage = nullptr;
};


