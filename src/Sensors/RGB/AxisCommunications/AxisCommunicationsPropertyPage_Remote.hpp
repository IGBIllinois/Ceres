/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams
 * connected to a remote computer.
 */

#pragma once

#include "AxisCommunicationsPropertyPage.hpp"
#include "AxisPropertiesNetDecoder.hpp"
#include "AxisPropertiesNetEncoder.hpp"

#include <optional>

/*
 * Base Property Page for the Axis Communications Web Cams
 */
class cAxisCommunicationsPropertyPage_Remote : public cAxisCommunicationsPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    protected cAxisPropertiesNetDecoder, protected cAxisPropertiesNetEncoder
{
public:
    cAxisCommunicationsPropertyPage_Remote(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Remote() = default;

    void connectToModel() override {};

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    void onConnect() override;
    void onGrabImagePressed() override;

protected:
    void createWidgets() override;
    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

protected:
    /*** Message Handlers from the networl decoder */
    void onModeMessage(uint8_t mode) override;
    void onImageSizeMessage(uint16_t width, uint16_t height) override;
    void onFrameRateMessage(uint8_t fps) override;
    void onLapseIntervalMessage(uint32_t interval_ms) override;

    void onTakePhotoReplyMessage(bool error)  override;

protected:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    uint8_t  mDefaultMode = 255;
    double   mDefaultFrameRate_fps = 0;
    uint32_t mDefaultLapseInterval_ms = 0;

    bool mReconnectActive = false;
};


/*
 * Property Page for the Axis Communications F44 system
 */

class cAxisCommunicationsPropertyPage_Remote_F44 : public cAxisCommunicationsPropertyPage_Remote
{
public:
    cAxisCommunicationsPropertyPage_Remote_F44(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Remote_F44() = default;

protected slots:
    void cameraIdTextChanged(const QString& text);

protected:
    void createWidgets() override;
    void doLayout(QVBoxLayout* pMainLayout) override;

    void doApply() override;

protected:
    /*** Message Handlers from the netowrk decoder */
    void onCameraIdMessage(uint8_t id) override;

    void onCurrentStateMessage(bool valid, uint8_t id,
        uint16_t width, uint16_t height, uint8_t fps) override;
    void onCurrentStateMessage(bool valid, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id) override;
    void onCurrentStateMessage(bool valid, uint8_t mode, uint8_t active_id,
        uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms,
        uint8_t min_id, uint8_t max_id, std::optional<double> min_fps, std::optional<double> max_fps) override;

private:
    QLabel* mpCameraIdLabel = nullptr;
    QComboBox* mpCameraId = nullptr;

    int mDefaultCameraId = -1;

    int mMinCameraId = 0;
    int mMaxCameraId = 0;

};
