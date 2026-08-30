/*
 * The GpsControllerNetEncoder is used to encode TCP packets to the GpsPropertiesNetDecoder.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_encoder.hpp"
#include "GpsTypes.hpp"

#include <vector>
#include <string>

class cGpsControllerNetEncoder : public cNetworkEncoder
{
public:
    cGpsControllerNetEncoder(std::size_t capacity);
    ~cGpsControllerNetEncoder() = default;

public:
    /*
     * Controller ----> Property Page
     */

     /*
      * The encode methods will encode the packet into the data buffer but will not send the
      * data unless the packet will not fit into the buffer.
      *
      * You need to call sendData to make sure the data is sent out!
      */
    void encodeReferenceDataMessage(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
        double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid);

    void encodeReferenceParametersMessage(uint16_t integration_time_sec, uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm);
    void encodeReferencePositionMessage(int x_mm, int y_mm, int z_mm, double error_mm, int count);

    void encodeReferenceStateMessage(gps::eReferenceState state);


    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
     */
    void sendReferenceDataMessage(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
        double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid);
    void sendReferencePositionMessage(int x_mm, int y_mm, int z_mm, double error_mm, int count);

    void sendReferenceParametersMessage(uint16_t integration_time_sec, uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm);

    void sendReferenceStateMessage(gps::eReferenceState state);
};


