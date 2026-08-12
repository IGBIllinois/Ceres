/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams
 * connected to the local computer.
 */

#pragma once

#include "AxisCommunicationsPropertyPage.hpp"

// Forward Declaration
class cAxisCommunicationsModel;
class cAxisCommunicationsModel_F44;


class cAxisCommunicationsPropertyPage_Local : public cAxisCommunicationsPropertyPage
{
    Q_OBJECT

public:
    cAxisCommunicationsPropertyPage_Local(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Local() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

signals:
    void requestMode(int mode);
    void requestImageSize(int width, int height);
    void requestFrameRate_Hz(double frame_rate_hz);
    void requestLapseInterval_ms(uint32_t interval_ms);
    void requestImage();
    void requestImages(bool update_view);
    void requestPhoto(bool update_view);
    void requestPhoto(bool update_view, bool auto_save);

public slots:
    void onPhotoTaken();
    void onModeChange(int mode);
    void onLapseIntervalChange(int interval_ms);
    void onFrameRateChange(int rate_fps);
    void onImageSizeChange(int width, int height);

protected:
    void createWidgets() override;
    void doLayout() override;

    void enableControls(bool enable) override;

    void showPage() override;

    void onGrabImagePressed() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

private:
    cAxisCommunicationsModel* mpModel;
};


class cAxisCommunicationsPropertyPage_Local_F44 : public cAxisCommunicationsPropertyPage_Local
{
    Q_OBJECT

public:
    cAxisCommunicationsPropertyPage_Local_F44(cAxisCommunicationsModel_F44* pModel, QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Local_F44() = default;

signals:
    void requestCameraID(int id);

public slots:
    void onCameraIdChange(int id);

protected:
    void doApply() override;

private:
    cAxisCommunicationsModel_F44* mpModel;
};


