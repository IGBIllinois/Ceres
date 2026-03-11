/*
 * The TeledyneFlirPropertyPage to control the Teledyne FLIR camera
 * connected to this computer.
 */

#pragma once

#include "TeledyneFlirPropertyPage.hpp"

#include <optional>

 // Forward Declaration
class cTeledyneFlirCameraModel;
class cTeledyneFlirCameraModel_T1K;


class cTeledyneFlirPropertyPage_Local : public cTeledyneFlirPropertyPage
{

public:
    cTeledyneFlirPropertyPage_Local(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage_Local() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

protected:
    void requestImage() override;

protected:
    void createWidgets() override;
    void doLayout() override;

    void enableControls(bool enable) override;

    void showPage() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

private:
    cTeledyneFlirCameraModel* mpModel;
};


class cTeledyneFlirPropertyPage_T1K_Local : public cTeledyneFlirPropertyPage_Local
{
public:
    cTeledyneFlirPropertyPage_T1K_Local(cTeledyneFlirCameraModel_T1K* pModel, QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage_T1K_Local() = default;

protected:
    void showPage() override;

private:
    cTeledyneFlirCameraModel_T1K* mpModel;
};

