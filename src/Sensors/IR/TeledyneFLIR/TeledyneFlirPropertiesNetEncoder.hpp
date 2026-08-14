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
    void sendQueryMode();
    void sendQueryImageSize();
    void sendQueryFrameRate();
    void sendQueryFrameInterval();
    void sendQueryThermalRange();

    void sendSetMode(uint8_t mode);
    void sendSetImageSize(uint16_t width, uint16_t height);
    void sendSetFrameRate_fps(double fps);
    void sendSetLapseInterval_ms(uint32_t interval_ms);
    void sendGrabImage();
    void sendTakePhoto(bool update_view);
    void sendTakePhoto(bool update_view, bool save_image);

    void sendSaveState();
    void sendRestoreState();
};


