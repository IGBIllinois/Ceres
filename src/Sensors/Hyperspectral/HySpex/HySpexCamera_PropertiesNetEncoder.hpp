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
    void encodeQueryState();
    void encodeQueryLensNames();
    void encodeQueryShutterState();
    void encodeAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void encodeLensName(const std::string& lens_name);
    void encodeNumOfBackgrounds(int num_backgrounds);
    void encodeCalcBackground();
    void encodeStopBackground();

    void encodeOpenShutter();
    void encodeCloseShutter();

    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
     */
    void sendQueryState();
    void sendQueryLensNames();
    void sendQueryShutterState();
    void sendAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us);
    void sendLensName(const std::string& lens_name);
    void sendNumOfBackgrounds(int num_backgrounds);
    void sendCalcBackground();
    void sendStopBackground();

    void sendOpenShutter();
    void sendCloseShutter();
};


