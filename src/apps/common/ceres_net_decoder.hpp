/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <string>

class cCeresNetDecoder : public cNetworkDecoder
{
public:
    cCeresNetDecoder() = default;
    virtual ~cCeresNetDecoder() = default;

protected:
    /*
     * The Ceres Remote Client sends these packets and should never receive them!
     */
    virtual void onExperimentInfoReply() = 0;
    virtual void onDataFileState(bool is_open) = 0;

    virtual void onStatusMessage(const std::string& msg) = 0;
    virtual void onLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg) = 0;
    virtual void onSensorStatus(const std::string& sensor, const std::string& status) = 0;
    virtual void onSensorNameChange(const std::string& old_name, const std::string& new_name) = 0;
    virtual void onSensorPropertyConnectInfo(const std::string& sensor, 
        const std::string& model, uint32_t version,
        const std::string& name, const std::string& ip_address, uint16_t port) = 0;

    virtual void onUnknownID(uint16_t id) = 0;

private:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


