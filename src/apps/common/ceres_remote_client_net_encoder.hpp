
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cCeresRemoteClientNetEncoder : public cNetworkEncoder
{

public:
    cCeresRemoteClientNetEncoder(std::size_t capacity);
    ~cCeresRemoteClientNetEncoder() = default;

    void encodeSensorStatus(const std::string& sensor, const std::string& status);

    /*
     * Ceres Remote Client ----> Ceres
     */
    void sendDataFileState(bool is_open);
    void sendStatusMessage(const std::string& msg);
    void sendLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg);
    void sendSensorStatus(const std::string& sensor, const std::string& status);
    void sendSensorNameChange(const std::string& old_name, const std::string& new_name);

};


