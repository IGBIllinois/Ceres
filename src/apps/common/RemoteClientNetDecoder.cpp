

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
    case ePacketType::TREATMENT:
    {
        ExperimentTreatment_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onTreatment(to_treatment_1(packet));
        break;
    }
    case ePacketType::EXPERIMENT_INFO_REPLY:
    {
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
    case ePacketType::DATA_RECORDING_STATE:
    {
        break;
    }
    case ePacketType::SPIDER_CAM_DATA:
    {
        Spidercam_Position_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onSpidercamPosition(to_spidercam_position_1(packet));
        break;
    }
    case ePacketType::WEATHER_DATA:
    {
        WeatherData_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        sWeatherData_t data = to_weather_data_1(packet);
        onWeatherData(data.dataValid, data.wind_speed_mps, data.wind_direction_deg);
        break;
    }
    }
}

