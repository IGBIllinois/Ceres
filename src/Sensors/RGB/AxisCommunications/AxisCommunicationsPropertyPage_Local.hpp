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

public:
    cAxisCommunicationsPropertyPage_Local(cAxisCommunicationsModel* pModel, QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Local() = default;

public:

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
    cAxisCommunicationsModel* mpModel;
};


