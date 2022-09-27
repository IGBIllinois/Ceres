

#include "ceres_net_decoder.hpp"
#include "remote_client_utils.hpp"
#include "net_buffer.hpp"

void cCeresNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::EXPERIMENT_INFO_REPLY:
    {
        break;
    }
    case ePacketType::DATA_FILE_STATE:
    {
        FileOpenState_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onDataFileState(to_file_open_state_1(packet));
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
    case ePacketType::SENSOR_PROPERTY_CONNECT_INFO:
    {
        SensorPropertyConnectInfo_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        sSensorPropertyConnectInfo_t data = to_sensor_property_connect_info_1(packet);
        onSensorPropertyConnectInfo(data.sensor, data.version, data.ip_address, data.port);
        break;
    }
    }
}
