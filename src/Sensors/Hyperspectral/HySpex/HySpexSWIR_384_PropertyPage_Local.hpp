/*
 * The HySpexSWIR_384_PropertyPage to control the HySpex SWIR 384
 * connected to the local computer.
 */

#pragma once

#include "HySpexSWIR_384_PropertyPage.hpp"


class cHySpexSWIR_384_PropertyPage_Local : public cHySpexSWIR_384_PropertyPage
{

public:
    cHySpexSWIR_384_PropertyPage_Local(QWidget* parent = nullptr);
    ~cHySpexSWIR_384_PropertyPage_Local() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    void createWidgets() override;
    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void doCalcBackground() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

protected:
    void sendChangedData();
    void queryState();
    void queryLensNames();
    void setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void setLensName(const std::string& lens_name);
    void setNumOfBackgrounds(int num_backgrounds);
    void calcBackground();

private:
    bool mAcquisitionParametersValid = false;
    bool mBackgroundValid = false;
};


