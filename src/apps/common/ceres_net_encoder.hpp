
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>

// Forward Declarations
namespace spidercam
{
    struct sPosition;
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

    void sendOpenDataFile(const std::string& fileName);
    void sendCloseDataFile();

    void sendSpidercamPosition(const spidercam::sPosition& pos);
    void sendWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg);
};

