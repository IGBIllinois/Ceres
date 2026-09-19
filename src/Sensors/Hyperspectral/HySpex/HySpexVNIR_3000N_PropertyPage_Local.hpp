/*
 * The HySpexVNIR_3000N_PropertyPage to control the HySpex VNIR 3000N
 * connected to the local computer.
 */

#pragma once

#include "HySpexVNIR_3000N_PropertyPage.hpp"

class cHySpexVNIR_3000N_PropertyPage_Local : public cHySpexVNIR_3000N_PropertyPage
{
public:
    cHySpexVNIR_3000N_PropertyPage_Local(QWidget* parent = nullptr);
    ~cHySpexVNIR_3000N_PropertyPage_Local() = default;

    void connectToModel() override;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    void createWidgets() override;
    void enableControls(bool enable) override;
    void buttonClicked(QAbstractButton* button) override;

    void showPage() override;

    void changeShutterState() override;
    void doCalcBackground() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

private:
    //void sendChangedData();
    //void queryState();
    //void queryLensNames();
    //void setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    //void setLensName(const std::string& lens_name);
    //void setNumOfBackgrounds(int num_backgrounds);
    //void calcBackground();

private:
    bool mAcquisitionParametersValid = false;
    bool mBackgroundValid = false;
};


