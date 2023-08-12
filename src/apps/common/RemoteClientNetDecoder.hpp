/*
 * The SpidercamNetDecoder is used to decode Spidercam based TCP packets 
 * from the main Ceres application.
 * You will need to override various virtual methods!
 */

#pragma once

#include "net_packet_decoder.hpp"

#include <spidercam/spidercam_types.hpp>

#include <string>
#include <ctime>


class cCeresRemoteClientNetDecoder : public cNetworkDecoder
{
public:

    cCeresRemoteClientNetDecoder() = default;
    virtual ~cCeresRemoteClientNetDecoder() = default;

protected:
    virtual void onExperimentInfo(const std::string& title,
        const std::string& researcher, const std::string& cultivar, 
        const std::string& doc) = 0;

    virtual void onExperimentInfo(const std::string& title,
        const std::string& researcher, const std::string& species, 
        const std::string& cultivar, const std::string& doc) = 0;

    virtual void onPrincipalInvestigator(const std::string& pi) = 0;

    virtual void onStartOfResearcherList() = 0;
    virtual void onEndOfResearcherList() = 0;
    virtual void onResearcher(const std::string& researcher) = 0;

    virtual void onConstructName(const std::string& name) = 0;

    virtual void onStartOfEventNumberList() = 0;
    virtual void onEndOfEventNumberList() = 0;
    virtual void onEventNumber(const std::string& event_num) = 0;

    virtual void onFieldDesign(const std::string& comment) = 0;
    virtual void onPlantingDate(std::time_t date) = 0;
    virtual void onHarvestDate(std::time_t date) = 0;

    virtual void onStartOfTreatmentList() = 0;
    virtual void onEndOfTreatmentList() = 0;
    virtual void onTreatment(const std::string& treatment) = 0;

    virtual void onStartOfCommentList() = 0;
    virtual void onEndOfCommentList() = 0;
    virtual void onComment(const std::string& comment) = 0;

    virtual void onPermitInfo(const std::string& permit) = 0;

    virtual void onEndOfExperimentInfo() = 0;

    virtual void onStartExperiment() = 0;
    virtual void onStopExperiment() = 0;

    virtual void onOpenDataFile(const std::string& fileName) = 0;
    virtual void onCloseDataFile() = 0;

    virtual void onStartDataRecording() = 0;
    virtual void onStopDataRecording() = 0;

    virtual void onSpidercamPosition(const spidercam::sPosition_1_t& pos) = 0;

    virtual void onWindData(bool valid, double wind_speed_mps, double wind_direction_deg) = 0;
    virtual void onTemperatureData(double temp_C) = 0;
    virtual void onRelativeHumidityData(double rh_pct) = 0;
    virtual void onParData(double par_umole) = 0;

/*
 * The Ceres Remote Client application sends these packets and should never receive them!
 */
private:
    void processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer) override final;

};


