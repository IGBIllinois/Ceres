
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>
#include <ctime>

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

    void sendPrincipalInvestigator(const std::string& pi);
    void sendResearcher(const std::string& researcher);
    void sendConstructName(const std::string& name);
    void sendEventNumber(const std::string& event_num);
    void sendFieldDesign(const std::string& design);
    void sendPlantingDate(std::time_t date);
    void sendHarvestDate(std::time_t date);
    void sendTreatment(const std::string& treatment);
    void sendComment(const std::string& comment);
    void sendPermitInfo(const std::string& permit);

    void sendStartExperiment();
    void sendStopExperiment();

    void sendOpenDataFile(const std::string& fileName);
    void sendCloseDataFile();

    void sendStartDataRecording();
    void sendStopDataRecording();

    void sendSpidercamPosition(const spidercam::sPosition_1_t& pos);
    void sendWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg);
};

