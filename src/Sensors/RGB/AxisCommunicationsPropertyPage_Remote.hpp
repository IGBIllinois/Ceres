/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams
 * connected to a remote computer.
 */

#pragma once

#include "AxisCommunicationsPropertyPage.hpp"
#include "AxisPropertiesNetDecoder.hpp"
#include "AxisPropertiesNetEncoder.hpp"

class cAxisCommunicationsPropertyPage_Remote : public cAxisCommunicationsPropertyPage,
    public cSensorPropertyPageRemoteInterface, 
    private cAxisPropertiesNetDecoder, private cAxisPropertiesNetEncoder
{

public:
    cAxisCommunicationsPropertyPage_Remote(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage_Remote() = default;

public:
    bool queryState() override;
    void onCurrentState(bool valid, const std::string& mode, double min_deg, double max_deg) override;

protected:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

};


