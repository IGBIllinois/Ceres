
#pragma once

#include "net_packet_encoder.hpp"
#include <cstddef>
#include <ctime>
#include <vector>

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

    /*
     * The encode methods will encode the packet into the data buffer but will not send the
     * data unless the packet will not fit into the buffer.
     * 
     * You need to call sendData to make sure the data is sent out!
     */
    void encodeExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& cultivar, const std::string& doc);

    void encodeExperimentInfo(const std::string& title, const std::string& researcher,
        const std::string& species, const std::string& cultivar, const std::string& doc);

    void encodeTitle(const std::string& title);
    void encodeDocument(const std::string& doc);
    void encodeSpecies(const std::string& species);
    void encodeCultivar(const std::string& cultivar);
    void encodePrincipalInvestigator(const std::string& pi);
    void encodeResearcher(const std::string& researcher);
    void encodeResearchers(const std::vector<std::string>& researchers);
    void encodeConstructName(const std::string& name);
    void encodeEventNumbers(const std::vector<std::string>& event_numbers);
    void encodeFieldDesign(const std::string& design);
    void encodePlantingDate(std::time_t date);
    void encodeHarvestDate(std::time_t date);
    void encodeTreatments(const std::vector<std::string>& treatments);
    void encodeComments(const std::vector<std::string>& comments);
    void encodePermitInfo(const std::string& permit);

    void encodeEndOfExperimentInfo();

    void encodeSpidercamPosition(const spidercam::sPosition_1_t& pos);
    void encodeWindData(bool valid, double wind_speed_mps, double wind_direction_deg);
    void encodeTemperatureData(double temp_C);
    void encodeRelativeHumidityData(double rh_pct);
    void encodeParData(double par_umole);


    /*
     * The send methods will encode the packet into the data buffer and will automatically
     * send the data.
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

    void sendEndOfExperimentInfo();

    void sendStartExperiment();
    void sendStopExperiment();

    void sendOpenDataFile(const std::string& fileName);
    void sendCloseDataFile();

    void sendStartDataRecording();
    void sendStopDataRecording();

    void sendSpidercamPosition(const spidercam::sPosition_1_t& pos);
    void sendWindData(bool valid, double wind_speed_mps, double wind_direction_deg);
    void sendTemperatureData(double temp_C);
    void sendRelativeHumidityData(double rh_pct);
    void sendParData(double par_umole);
};

