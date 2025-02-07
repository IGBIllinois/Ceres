

#include "GpsControllerNetEncoder.hpp"
#include "gps_packet_utils.hpp"
#include "gps_properties.pb.h"


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

void cGpsControllerNetEncoder::encodeReferenceParameters(uint16_t integration_time_sec, uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm)
{
    if (encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer) < 0)
    {
        sendData();
        encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeReferenceState(gps::eReferenceState state)
{
    gps_eReferenceReply reply;

    switch (state)
    {
    case gps::eReferenceState::ABORT:
        reply = gps_eReferenceReply::eReferenceReply_ABORTED;
        break;
    case gps::eReferenceState::COMPLETE_FAILED:
        reply = gps_eReferenceReply::eReferenceReply_FAILED;
        break;
    case gps::eReferenceState::COMPLETE_GOOD:
        reply = gps_eReferenceReply::eReferenceReply_GOOD;
        break;
    case gps::eReferenceState::START:
    case gps::eReferenceState::PENDING:
        reply = gps_eReferenceReply::eReferenceReply_PENDING;
        break;
    case gps::eReferenceState::WAITING:
        reply = gps_eReferenceReply::eReferenceReply_IDLE;
        break;
    }

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

void cGpsControllerNetEncoder::sendReferenceParameters(uint16_t integration_time_sec, uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm)
{
    encode_reference_parameters_reply(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer);
    sendData();
}

void cGpsControllerNetEncoder::sendReferenceState(gps::eReferenceState state)
{
    gps_eReferenceReply reply;

    switch (state)
    {
    case gps::eReferenceState::ABORT:
        reply = gps_eReferenceReply::eReferenceReply_ABORTED;
        break;
    case gps::eReferenceState::COMPLETE_FAILED:
        reply = gps_eReferenceReply::eReferenceReply_FAILED;
        break;
    case gps::eReferenceState::COMPLETE_GOOD:
        reply = gps_eReferenceReply::eReferenceReply_GOOD;
        break;
    case gps::eReferenceState::START:
    case gps::eReferenceState::PENDING:
        reply = gps_eReferenceReply::eReferenceReply_PENDING;
        break;
    case gps::eReferenceState::WAITING:
        reply = gps_eReferenceReply::eReferenceReply_IDLE;
        break;
    }

    encode_reference_reply(reply, mBuffer);
    sendData();
}

