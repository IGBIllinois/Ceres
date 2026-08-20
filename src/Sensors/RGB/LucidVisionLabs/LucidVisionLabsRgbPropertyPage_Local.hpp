/*
 * The LucidVisionLabsRgbPropertyPage to control the Lucid Vision Labs Triton RGB cameras
 * connected to the local computer.
 */

#pragma once

#include "LucidVisionLabsRgbPropertyPage.hpp"

// Forward Declaration
class cLucidVisionLabsRgbModel;
class cLucidVisionLabsRgbModel_Triton;


class cLucidVisionLabsRgbPropertyPage_Local : public cLucidVisionLabsRgbPropertyPage
{
    Q_OBJECT

public:
    cLucidVisionLabsRgbPropertyPage_Local(cLucidVisionLabsRgbModel* pModel, QWidget* parent = nullptr);
    ~cLucidVisionLabsRgbPropertyPage_Local() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

    void connectToModel() override;

signals:
    void requestPhoto(bool update_view);
//    void requestPhoto(bool update_view, bool auto_save);

signals:
    void pushStreamState();
    void popStreamState();
    void changeMode(int mode);
    void changeFrameRate_Hz(double frame_rate_hz);
    void changeLapseInterval_ms(uint32_t interval_ms);
    void changePixelFormat(int mode);
    void changeExposure(int mode, double exposureTime_us);
    void changeGain(int mode, double gain_dB);
    void changeBalanceWhiteAuto(int mode);
    void changeGamma(bool enable, double gamma);


public slots:
    void photoTaken();

    void pixelFormatUpdated(int mode);
    void exposureUpdated(int mode, double exposureTime_us);
    void gainUpdated(int mode, double gain_dB);
    void balanceWhiteAutoUpdated(int mode);
    void gammaUpdated(bool enabled, double gamma);

protected:
    void exposureTimeSelectorChanged(const QString&) override;
    void exposureAutoModeChanged(const QString&) override;
    void pixelFormatChanged(const QString&) override;
    void gainAutoModeChanged(const QString&) override;
    void balanceWhiteAutoModeChanged(const QString&) override;
    void gammaEnableChanged(bool check) override;

protected:
    void createWidgets() override;
    void doLayout() override;

    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void onGrabImagePressed() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

private:
    cLucidVisionLabsRgbModel* mpModel;
};

class cLucidVisionLabsRgbPropertyPage_Local_Triton : public cLucidVisionLabsRgbPropertyPage_Local
{
    Q_OBJECT

public:
    cLucidVisionLabsRgbPropertyPage_Local_Triton(cLucidVisionLabsRgbModel_Triton* pModel, QWidget* parent = nullptr);
    ~cLucidVisionLabsRgbPropertyPage_Local_Triton() = default;

    void connectToModel() override;

protected:
    void showPage() override;

private:
    cLucidVisionLabsRgbModel_Triton* mpModel;
};


