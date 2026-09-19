/*
 * The HySpexCamera_ControllerNetDecoder is used to decode TCP packets
 * from the HySpexCamera_PropertiesNetEncoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"
//#include "hyspex_properties.pb.h"

#include <string>

namespace hyspex
{
    enum class ePacketType : uint16_t;
}


class cHySpexCamera_ControllerNetDecoder : public cNetworkDecoder
{
public:

    cHySpexCamera_ControllerNetDecoder() = default;
    virtual ~cHySpexCamera_ControllerNetDecoder() = default;

protected:
    /*
     * Property Page ----> Controller
     */
    virtual void onQueryStateMessage() = 0;
    virtual void onQueryLensNamesMessage() = 0;
    virtual void onQueryShutterStateMessage() = 0;
    virtual void onQueryBackgroundStateMessage() = 0;
    virtual void onSetAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us) = 0;
    virtual void onSetLensNameMessage(const std::string& lens_name) = 0;
    virtual void onSetNumOfBackgroundsMessage(int num_backgrounds) = 0;
    virtual void onCalcBackgroundMessage() = 0;
    virtual void onStopBackgroundMessage() = 0;

    virtual void onOpenShutterMessage() = 0;
    virtual void onCloseShutterMessage() = 0;

protected:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;
    virtual void processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) = 0;
};


