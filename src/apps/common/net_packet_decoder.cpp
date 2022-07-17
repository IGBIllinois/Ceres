

#include "net_packet_decoder.hpp"
#include "net_buffer.hpp"
#include "packet_utils.hpp"

void cNetworkDecoder::decode(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    ceres::net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

    sPacketHeader_t hdr;

    while (buffer.size() > 0)
    {
        buffer >> hdr;
     
        if (buf_length < hdr.length)
        { 
            break;
        }

        switch (hdr.id)
        {
            case ePacketType::UNKNOWN:
            default:
            {
                break;
            }
            case ePacketType::EXPERIMENT_INFO:
            {
                ExperimentInfo_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                sExperimentInfo_t data = to_experiment_info_1(packet);
                onExperimentInfo(data.title, data.researcher, data.cultivar, data.doc);
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
            case ePacketType::DATA_FILE_STATE:
            {
                FileOpenState_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                onDataFileState(to_file_open_state_1(packet));
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
                onStartExperiment();
                break;
            }
            case ePacketType::DATA_RECORDING_STATE:
            {
                break;
            }
            case ePacketType::STATUS_MESSAGE:
            {
                StatusMessage_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                onStatusMessage(to_status_message_1(packet));
                break;
            }
            case ePacketType::LOG_MESSAGE:
            {
                LogMessage_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                auto data = to_log_message_1(packet);
                onLogMessage(data.msg_type, data.device, data.message);
                break;
            }
            case ePacketType::SENSOR_STATUS:
            {
                SensorStatus_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                sSensorStatus_t data = to_sensor_status_1(packet);
                onSensorStatus(data.name, data.status);
                break;
            }
            case ePacketType::SENSOR_NAME_CHANGE:
            {
                SensorNameChange_1 packet;
                packet.ParseFromArray(buffer.data(), hdr.length);
                sSensorNameChange_t data = to_sensor_name_change_1(packet);
                onSensorNameChange(data.old_name, data.new_name);
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

        buffer.advance(hdr.length);
    }
}


