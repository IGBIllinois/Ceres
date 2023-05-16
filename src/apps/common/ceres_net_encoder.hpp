
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>

// Forward Declarations
namespace spidercam
{
    struct sPosition_1_t;
}


class cCeresNetEncoder : public cNetworkEncoder
{

public:
    cCeresNetEncoder(std::size_t capacity);
    ~cCeresNetEncoder() = default;

    /*
     * Ceres ----> Ceres Remote Client
     */
    void sendExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& cultivar, const std::string& doc);

    void sendExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& species, const std::string& cultivar, const std::string& doc);

    void sendTreatment(const std::string& treatment);

    void sendStartExperiment();
    void sendStopExperiment();

    void sendOpenDataFile(const std::string& fileName);
    void sendCloseDataFile();

    void sendStartDataRecording();
    void sendStopDataRecording();

    void sendSpidercamPosition(const spidercam::sPosition_1_t& pos);
    void sendWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg);
};

