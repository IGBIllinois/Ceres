/*
 * The NetworkDecoder is used to decode network based packets 
 * You will need to override various virtual methods!
 */

#pragma once

#include <spidercam/spidercam_types.hpp>


#include <cstddef>


class cNetworkDecoder
{
public:

    cNetworkDecoder() = default;
    virtual ~cNetworkDecoder() = default;

    /**
     * @brief The decode method convert the byte stream from a TCP receiver port.
     * The decode the various packets and call the various pure virtual 
     * methods.
     */
    void decode(const void* pBuffer, std::size_t buf_length);

private:
    virtual void onExperimentInfo(const std::string& title, 
        const std::string& researcher, const std::string& cultivar, const std::string& doc)= 0;

    virtual void onStartExperiment() = 0;
    virtual void onStopExperiment() = 0;

    virtual void onOpenDataFile(const std::string& fileName) = 0;
    virtual void onCloseDataFile() = 0;
    virtual void onDataFileState(bool is_open) = 0;

    virtual void onStartDataRecording() = 0;
    virtual void onStopDataRecording() = 0;

    virtual void onSpidercamPosition(const spidercam::sPosition_1_t& pos) = 0;

    virtual void onWeatherData(bool valid, double wind_speed_mps, 
        double wind_direction_deg) = 0;
};


