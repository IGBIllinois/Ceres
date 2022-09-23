

#include "OusterPropertiesNetEncoder.hpp"
#include "packet_utils.hpp"


#if 0
cCeresRemoteClientNetEncoder::cCeresRemoteClientNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cCeresRemoteClientNetEncoder::encodeSensorStatus(const std::string& sensor, const std::string& status)
{
    encode_sensor_status(sensor, status, mBuffer);
}

void cCeresRemoteClientNetEncoder::sendDataFileState(bool is_open)
{
    encode_file_open_state(is_open, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendStatusMessage(const std::string& msg)
{
    encode_status_message(msg, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendLogMessage(uint8_t msg_type,
                        const std::string& device, const std::string& msg)
{
    encode_log_message(msg_type, device, msg, mBuffer);
    sendData();

}

void cCeresRemoteClientNetEncoder::sendSensorStatus(const std::string& sensor, const std::string& status)
{
    encode_sensor_status(sensor, status, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorNameChange(const std::string& old_name, const std::string& new_name)
{
    encode_sensor_name_change(old_name, new_name, mBuffer);
    sendData();
}
#endif
