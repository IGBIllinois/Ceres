/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

class cCeresNetDecoder : public cNetworkDecoder
{
public:
    cCeresNetDecoder() = default;
    virtual ~cCeresNetDecoder() = default;

/*
 * The Ceres application sends these packets and should never receive them!
 */
private:
    void onExperimentInfo(const std::string&, const std::string&, const std::string&, const std::string&) override final {};
    void onStartExperiment() override final {};
    void onStopExperiment() override final {};

    void onOpenDataFile(const std::string&) override final {};
    void onCloseDataFile() override final {};
    void onStartDataRecording() override final {};
    void onStopDataRecording() override final {};

    void onSpidercamPosition(const spidercam::sPosition& pos) override final {};
    void onWeatherData(bool valid, double wind_speed_mph, double wind_direction_deg) override final {};
};


