/*
 * The HySpexVNIR_3000N_PropertyPage to control the HySpex VNIR 3000N
 * connected to a remote computer.
 */

#pragma once

#include "HySpexVNIR_3000N_PropertyPage.hpp"
#include "HySpexVNIR_3000N_PropertiesNetDecoder.hpp"
#include "HySpexVNIR_3000N_PropertiesNetEncoder.hpp"

class cHySpexVNIR_3000N_PropertyPage_Remote : public cHySpexVNIR_3000N_PropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cHySpexVNIR_3000N_PropertiesNetDecoder, private cHySpexVNIR_3000N_PropertiesNetEncoder
{

public:
    cHySpexVNIR_3000N_PropertyPage_Remote(QWidget* parent = nullptr);
    ~cHySpexVNIR_3000N_PropertyPage_Remote() = default;

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


