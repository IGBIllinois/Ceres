/*
 * The HySpexCamera_PropertiesNetEncoder is used to encode TCP packets
 * to send to the HySpexCamera_ControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cHySpexCamera_PropertiesNetEncoder : public cNetworkEncoder
{

public:
    cHySpexCamera_PropertiesNetEncoder(std::size_t capacity);
    ~cHySpexCamera_PropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    /*
     * The encode methods will encode the packet into the data buffer but will not send the
     * data unless the packet will not fit into the buffer.
     *
     * You need to call sendData to make sure the data is sent out!
     */
    void encodeQueryStateMessage();
    void encodeQueryLensNamesMessage();
    void encodeQueryShutterStateMessage();
    void encodeQueryBackgroundStateMessage();
    void encodeAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void encodeLensNameMessage(const std::string& lens_name);
    void encodeNumOfBackgroundsMessage(int num_backgrounds);
    void encodeCalcBackgroundMessage();
    void encodeStopBackgroundMessage();

    void encodeOpenShutterMessage();
    void encodeCloseShutterMessage();

    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
     */
    void sendQueryStateMessage();
    void sendQueryLensNamesMessage();
    void sendQueryShutterStateMessage();
    void sendQueryBackgroundStateMessage();
    void sendAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void sendLensNameMessage(const std::string& lens_name);
    void sendNumOfBackgroundsMessage(int num_backgrounds);
    void sendCalcBackgroundMessage();
    void sendStopBackgroundMessage();

    void sendOpenShutterMessage();
    void sendCloseShutterMessage();
};


