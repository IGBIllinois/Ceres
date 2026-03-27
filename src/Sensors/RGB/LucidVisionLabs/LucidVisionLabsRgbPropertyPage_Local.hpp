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

public:
    cLucidVisionLabsRgbPropertyPage_Local(cLucidVisionLabsRgbModel* pModel, QWidget* parent = nullptr);
    ~cLucidVisionLabsRgbPropertyPage_Local() = default;

protected:
    void exposureTimeSelectorChanged(const QString&) override;
    void exposureAutoModeChanged(const QString&) override;
    void pixelFormatChanged(const QString&) override;
    void gainAutoModeChanged(const QString&) override;
    void balanceWhiteAutoModeChanged(const QString&) override;

protected:
    void createWidgets() override;
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


