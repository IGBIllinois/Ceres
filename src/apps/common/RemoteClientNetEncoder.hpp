
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cCeresRemoteClientNetEncoder : public cNetworkEncoder
{

public:
    cCeresRemoteClientNetEncoder(std::size_t capacity);
    ~cCeresRemoteClientNetEncoder() = default;


    /*
     * Ceres Remote Client ----> Ceres
     */

    /* You will need to call sendData to actually send the packet information */
    void encodeSensorStatus(const std::string& sensor, const std::string& status);
    void encodeSensorPropertyConnectInfo(const std::string& sensor, 
        const std::string& model, uint32_t version,
        const std::string& name, const std::string& ip_address, uint16_t port);

    /* Automatically encodes and calls sendData to send the packet information */
    void sendExperimentInfoReply();
    void sendDataFileState(bool is_open);
    void sendStatusMessage(const std::string& msg);
    void sendLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg);
    void sendSensorStatus(const std::string& sensor, const std::string& status);
    void sendSensorNameChange(const std::string& old_name, const std::string& new_name);
    void sendSensorPropertyConnectInfo(const std::string& sensor, const std::string& model,
        uint32_t version, const std::string& name, const std::string& ip_address, uint16_t port);
};


