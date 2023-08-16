

#include "RemoteClientNetDecoder.hpp"

#include "remote_client_utils.hpp"
#include "net_buffer.hpp"


void cCeresRemoteClientNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::EXPERIMENT_INFO:
    {
        switch (hdr.revision)
        {
        case 1:
        {
            ExperimentInfo_1 packet;
            packet.ParseFromArray(buffer.data(), hdr.length);
            sExperimentInfo_t data = to_experiment_info_1(packet);
            onExperimentInfo(data.title, data.researcher, data.cultivar, data.doc);
            break;
        }
        case 2:
        {
            ExperimentInfo_2 packet;
            packet.ParseFromArray(buffer.data(), hdr.length);
            sExperimentInfo_t data = to_experiment_info_2(packet);
            onExperimentInfo(data.title, data.researcher, data.species, data.cultivar, data.doc);
            break;
        }
        }

        break;
    }
    case ePacketType::START_OF_TREATMENT_LIST:
        onStartOfTreatmentList();
        break;
    case ePacketType::END_OF_TREATMENT_LIST:
        onEndOfTreatmentList();
        break;
    case ePacketType::TREATMENT:
    {
        ExperimentTreatment_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onTreatment(to_treatment_1(packet));
        break;
    }
    case ePacketType::PRINCIPAL_INVESTIGATOR:
    {
        PrincipalInvestigator_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onPrincipalInvestigator(to_principal_investigator_1(packet));
        break;
    }
    case ePacketType::START_OF_RESEARCHER_LIST:
        onStartOfResearcherList();
        break;
    case ePacketType::END_OF_RESEARCHER_LIST:
        onEndOfResearcherList();
        break;
    case ePacketType::RESEARCHER:
    {
        Researcher_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onResearcher(to_researcher_1(packet));
        break;
    }
    case ePacketType::CONSTRUCT_NAME:
    {
        ConstructName_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onConstructName(to_construct_name_1(packet));
        break;
    }
    case ePacketType::START_OF_EVENT_NUMBER_LIST:
        onStartOfEventNumberList();
        break;
    case ePacketType::END_OF_EVENT_NUMBER_LIST:
        onEndOfEventNumberList();
        break;
    case ePacketType::EVENT_NUMBER:
    {
        EventNumber_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onEventNumber(to_event_number_1(packet));
        break;
    }
    case ePacketType::FIELD_DESIGN:
    {
        FieldDesign_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onFieldDesign(to_field_design_1(packet));
        break;
    }
    case ePacketType::PLANTING_DATE:
    {
        PlantingDate_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onPlantingDate(to_planting_date_1(packet));
        break;
    }
    case ePacketType::HARVEST_DATE:
    {
        HarvestDate_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onHarvestDate(to_harvest_date_1(packet));
        break;
    }
    case ePacketType::START_OF_COMMENT_LIST:
        onStartOfCommentList();
        break;
    case ePacketType::END_OF_COMMENT_LIST:
        onEndOfCommentList();
        break;
    case ePacketType::COMMENT:
    {
        ExperimentComment_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onComment(to_comment_1(packet));
        break;
    }
    case ePacketType::PERMIT_INFO:
    {
        ExperimentPermitInfo_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onPermitInfo(to_permit_info_1(packet));
        break;
    }
    case ePacketType::END_OF_EXPERIMENT_INFO:
    {
        onEndOfExperimentInfo();
        break;
    }
    case ePacketType::OPEN_DATA_FILE:
    {
        OpenDataFile_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onOpenDataFile(to_filename_1(packet));
        break;
    }
    case ePacketType::CLOSE_DATA_FILE:
    {
        onCloseDataFile();
        break;
    }
    case ePacketType::START_DATA_RECORDING:
    {
        onStartDataRecording();
        break;
    }
    case ePacketType::STOP_DATA_RECORDING:
    {
        onStopDataRecording();
        break;
    }
    case ePacketType::START_EXPERIMENT:
    {
        onStartExperiment();
        break;
    }
    case ePacketType::STOP_EXPERIMENT:
    {
        onStopExperiment();
        break;
    }
    case ePacketType::SPIDER_CAM_DATA:
    {
        auto data = to_spidercam_position_1(hdr.length, buffer);
        onSpidercamPosition(data);
        break;
    }
    case ePacketType::WIND_DATA:
    {
        sWindData_t data = to_wind_data_1(hdr.length, buffer);
        onWindData(data.dataValid, data.wind_speed_mps, data.wind_direction_deg);
        break;
    }
    case ePacketType::TEMPERATURE_DATA:
    {
        auto data = to_temperature_data_1(hdr.length, buffer);
        onTemperatureData(data);
        break;
    }
    case ePacketType::RELATIVE_HUMIDITY_DATA:
    {
        auto data = to_relative_humidity_data_1(hdr.length, buffer);
        onRelativeHumidityData(data);
        break;
    }
    case ePacketType::PAR_DATA:
    {
        auto data = to_par_data_1(hdr.length, buffer);
        onParData(data);
        break;
    }
    case ePacketType::EXPERIMENT_INFO_REPLY:
    case ePacketType::DATA_RECORDING_STATE:
    {
        break;
    }
    }
}

