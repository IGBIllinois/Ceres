/*
 * The TeledyneFlirPropertyPage to control the Teledyne FLIR camera
 * connected to this computer.
 */

#pragma once

#include "TeledyneFlirPropertyPage.hpp"

#include <optional>

 // Forward Declaration
class cTeledyneFlirCameraModel;


class cTeledyneFlirPropertyPage_Local : public cTeledyneFlirPropertyPage
{

public:
    cTeledyneFlirPropertyPage_Local(cTeledyneFlirCameraModel* pModel, QWidget* parent = nullptr);
    ~cTeledyneFlirPropertyPage_Local() = default;

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


