/*
 * The GpsPropertiesNetEncoder is used to encode TCP packets
 * to send to the GpsControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cGpsPropertiesNetEncoder : public cNetworkEncoder
{
public:
    cGpsPropertiesNetEncoder(std::size_t capacity);
    ~cGpsPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

     /*
      * The encode methods will encode the packet into the data buffer but will not send the
      * data unless the packet will not fit into the buffer.
      *
      * You need to call sendData to make sure the data is sent out!
      */
    void encodeQueryReferenceDataMessage();
    void encodeQueryReferenceStateMessage();
    void encodeQueryReferenceParametersMessage();
    void encodeReferenceParametersMessage(std::uint16_t min_integration_time_sec, std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm);
    void encodeCalcReferenceMessage();
    void encodeStopReferenceMessage();

    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
     */
    void sendQueryReferenceDataMessage();
    void sendQueryReferenceStateMessage();
    void sendQueryReferenceParametersMessage();
    void sendReferenceParametersMessage(std::uint16_t min_integration_time_sec, std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm);
    void sendCalcReferenceMessage();
    void sendStopReferenceMessage();
};




