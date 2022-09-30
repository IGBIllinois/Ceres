/*
 * The OusterPropertyPage to control the OUSTER LiDAR
 * connected to a remote computer.
 */

#pragma once

#include "OusterPropertyPage.hpp"
#include "OusterPropertiesNetDecoder.hpp"
#include "OusterPropertiesNetEncoder.hpp"

class cOusterPropertyPage_Remote : public cOusterPropertyPage, 
    public cSensorPropertyPageRemoteInterface, 
    private cOusterPropertiesNetDecoder, private cOusterPropertiesNetEncoder
{

public:
    cOusterPropertyPage_Remote(QWidget* parent = nullptr);
    ~cOusterPropertyPage_Remote() = default;

public:
    void onCurrentState(bool valid, const std::string& mode, double min_deg, double max_deg) override;

protected:
    int sendOutgoingData(const char* data, std::size_t len) override;

};


