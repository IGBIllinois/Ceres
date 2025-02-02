

#include "GpsControllerNetEncoder.hpp"
#include "gps_packet_utils.hpp"


using namespace gps;

cGpsControllerNetEncoder::cGpsControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/**   Encode Methods   */
void cGpsControllerNetEncoder::encodeReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
    if (encode_reference_data(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
        std_lat_rad, std_lng_rad, std_height_m, height_valid, mBuffer) < 0)
    {
        sendData();

        encode_reference_data(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
            std_lat_rad, std_lng_rad, std_height_m, height_valid, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeReferenceParameters(uint16_t integration_time_sec, uint16_t max_integration_time_sec)
{
    if (encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, mBuffer) < 0)
    {
        sendData();
        encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeReferenceReply(gps_eReferenceReply reply)
{
    if (encode_reference_reply(reply, mBuffer) < 0)
    {
        sendData();
        encode_reference_reply(reply, mBuffer);
    }
}

/**   Send Methods   */
void cGpsControllerNetEncoder::sendReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
    encode_reference_data(valid, avg_lat_rad, avg_lng_rad, avg_height_m,
        std_lat_rad, std_lng_rad, std_height_m, height_valid, mBuffer);

    sendData();
}

void cGpsControllerNetEncoder::sendReferenceParameters(uint16_t integration_time_sec, uint16_t max_integration_time_sec)
{
    encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, mBuffer);
    sendData();
}

void cGpsControllerNetEncoder::sendReferenceReply(gps_eReferenceReply reply)
{
    encode_reference_reply(reply, mBuffer);
    sendData();
}

