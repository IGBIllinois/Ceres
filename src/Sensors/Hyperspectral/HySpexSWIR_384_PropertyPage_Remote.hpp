/*
 * The HySpexSWIR_384_PropertyPage to control the HySpex SWIR 384
 * connected to a remote computer.
 */

#pragma once

#include "HySpexSWIR_384_PropertyPage.hpp"
#include "HySpexSWIR_384_PropertiesNetDecoder.hpp"
#include "HySpexSWIR_384_PropertiesNetEncoder.hpp"


class cHySpexSWIR_384_PropertyPage_Remote : public cHySpexSWIR_384_PropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cHySpexSWIR_384_PropertiesNetDecoder,
    private cHySpexSWIR_384_PropertiesNetEncoder
{

public:
    cHySpexSWIR_384_PropertyPage_Remote(QWidget* parent = nullptr);
    ~cHySpexSWIR_384_PropertyPage_Remote() = default;

public:
    void onCurrentState(bool valid) override;

protected:
    void onConnect() override;

protected:
    void showPage() override;
    void doOK() override;
    void doCancel() override;
    void doApply() override;

protected:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

};


