

#include "GpsPropertiesNetEncoder.hpp"
#include "gps_packet_utils.hpp"

using namespace gps;


cGpsPropertiesNetEncoder::cGpsPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{
}

/**   Encode Methods   */
void cGpsPropertiesNetEncoder::encodeQueryReferenceDataMessage()
{
    if (encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer) < 0)
    {
        sendData();
        encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeQueryReferenceStateMessage()
{
    if (encode_gps_query(gps_eQuery::eQUERY_REFERENCE_STATE, mBuffer) < 0)
    {
        sendData();
        encode_gps_query(gps_eQuery::eQUERY_REFERENCE_STATE, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeQueryReferenceParametersMessage()
{
    if (encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer) < 0)
    {
        sendData();
        encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeReferenceParametersMessage(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm)
{
    if (encode_reference_parameters_set(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer) < 0)
    {
        sendData();
        encode_reference_parameters_set(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeCalcReferenceMessage()
{
    if (encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer) < 0)
    {
        sendData();
        encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeStopReferenceMessage()
{
    if (encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer) < 0)
    {
        sendData();
        encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer);
    }
}


/**   Send Methods   */
void cGpsPropertiesNetEncoder::sendQueryReferenceDataMessage()
{
    encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendQueryReferenceStateMessage()
{
    encode_gps_query(gps_eQuery::eQUERY_REFERENCE_STATE, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendQueryReferenceParametersMessage()
{
    encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendReferenceParametersMessage(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm)
{
    encode_reference_parameters_set(integration_time_sec, max_integration_time_sec, ref_error_threshold_mm, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendCalcReferenceMessage()
{
    encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendStopReferenceMessage()
{
    encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer);
    sendData();
}


