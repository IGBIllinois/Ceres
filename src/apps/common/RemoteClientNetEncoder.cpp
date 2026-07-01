

#include "RemoteClientNetEncoder.hpp"
#include "remote_client_utils.hpp"


cCeresRemoteClientNetEncoder::cCeresRemoteClientNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cCeresRemoteClientNetEncoder::encodeSensorStatus(const std::string& sensor, const std::string& status)
{
    if (encode_sensor_status(sensor, status, mBuffer) < 0)
    {
        sendData();
        encode_sensor_status(sensor, status, mBuffer);
    }
}

void cCeresRemoteClientNetEncoder::encodeSensorStatus(const std::string& sensor, const std::string& instance, const std::string& status)
{
    if (encode_sensor_status(sensor, instance, status, mBuffer) < 0)
    {
        sendData();
        encode_sensor_status(sensor, instance, status, mBuffer);
    }
}

void cCeresRemoteClientNetEncoder::encodeSensorPropertyConnectInfo(const std::string& sensor,
    const std::string& model, uint32_t version, const std::string& name, const std::string& ip_address,
    uint16_t port)
{
    if (encode_sensor_property_connect_info(sensor, model, version, name, ip_address, port, mBuffer) < 0)
    {
        sendData();
        encode_sensor_property_connect_info(sensor, model, version, name, ip_address, port, mBuffer);
    }
}

void cCeresRemoteClientNetEncoder::encodeSensorPropertyConnectInfo(const std::string& sensor,
    const std::string& model, uint32_t version,
    const std::string& name, const std::string& instance,
    const std::string& ip_address, uint16_t port)
{
    if (encode_sensor_property_connect_info(sensor, model, version, name, instance, ip_address, port, mBuffer) < 0)
    {
        sendData();
        encode_sensor_property_connect_info(sensor, model, version, name, instance, ip_address, port, mBuffer);
    }
}

void cCeresRemoteClientNetEncoder::encodeRemoteThreadStatus(const eRemoteThread_STATUS status)
{
    if (encode_remote_thread_status(status, mBuffer) < 0)
    {
        sendData();
        encode_remote_thread_status(status, mBuffer);
    }
}


void cCeresRemoteClientNetEncoder::sendExperimentInfoReply()
{
    encode_experiment_info_reply(mBuffer);
    sendData();
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

void cCeresRemoteClientNetEncoder::sendLogMessage(uint8_t msg_type,
    const std::string& device, const std::string& instance, const std::string& msg)
{
    encode_log_message(msg_type, device, instance, msg, mBuffer);
    sendData();

}

void cCeresRemoteClientNetEncoder::sendSensorStatus(const std::string& sensor, const std::string& status)
{
    encode_sensor_status(sensor, status, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorStatus(const std::string& sensor, const std::string& instance, const std::string& status)
{
    encode_sensor_status(sensor, instance, status, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorNameChange(const std::string& old_name, const std::string& new_name)
{
    encode_sensor_name_change(old_name, new_name, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorNameChange(const std::string& old_name, const std::string& new_name, const std::string& instance)
{
    encode_sensor_name_change(old_name, new_name, instance, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorPropertyConnectInfo(const std::string& sensor, 
    const std::string& model, uint32_t version, const std::string& name, const std::string& ip_address, uint16_t port)
{
    encode_sensor_property_connect_info(sensor, model, version, name, ip_address, port, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendSensorPropertyConnectInfo(const std::string& sensor,
    const std::string& model, uint32_t version,
    const std::string& name, const std::string& instance, 
    const std::string& ip_address, uint16_t port)
{
    encode_sensor_property_connect_info(sensor, model, version, name, instance, ip_address, port, mBuffer);
    sendData();
}

void cCeresRemoteClientNetEncoder::sendRemoteThreadStatus(const eRemoteThread_STATUS status)
{
    encode_remote_thread_status(status, mBuffer);
    sendData();
}
