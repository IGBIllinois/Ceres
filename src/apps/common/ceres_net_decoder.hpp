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
     * The Ceres application sends these packets and should never receive them!
     */
    virtual void onDataFileState(bool is_open) = 0;

    virtual void onStatusMessage(const std::string& msg) = 0;
    virtual void onLogMessage(uint8_t msg_type, const std::string& device, const std::string& msg) = 0;
    virtual void onSensorStatus(const std::string& sensor, const std::string& status) = 0;
    virtual void onSensorNameChange(const std::string& old_name, const std::string& new_name) = 0;

private:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
};


