/*
 * The TeledyneFlirPropertiesNetEncoder is used to encode TCP packets
 * to send to the TeledyneFlirControllerNetDecoder.
 */


#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>


class cTeledyneFlirPropertiesNetEncoder : public cNetworkEncoder
{

public:
    cTeledyneFlirPropertiesNetEncoder(std::size_t capacity);
    ~cTeledyneFlirPropertiesNetEncoder() = default;

    /*
     * Property Page ----> Controller
     */

    void sendQueryState();
    void sendSetCameraId(uint8_t id);
    void sendSetImageSize(uint16_t width, uint16_t height);
    void sendSetFrameRate_fps(uint8_t fps);
    void sendGrabImage();
};


