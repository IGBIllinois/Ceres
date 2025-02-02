

#include "GpsPropertiesNetEncoder.hpp"
#include "gps_packet_utils.hpp"

using namespace gps;


cGpsPropertiesNetEncoder::cGpsPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{
}

/**   Encode Methods   */
void cGpsPropertiesNetEncoder::encodeQueryReferenceData()
{
    if (encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer) < 0)
    {
        sendData();
        encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeQueryReferenceParameters()
{
    if (encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer) < 0)
    {
        sendData();
        encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeReferenceParameters(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec)
{
    if (encode_set_reference_parameters(integration_time_sec, max_integration_time_sec, mBuffer) < 0)
    {
        sendData();
        encode_set_reference_parameters(integration_time_sec, max_integration_time_sec, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeCalcReference()
{
    if (encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer) < 0)
    {
        sendData();
        encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeStopReference()
{
    if (encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer) < 0)
    {
        sendData();
        encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer);
    }
}


/**   Send Methods   */
void cGpsPropertiesNetEncoder::sendQueryReferenceData()
{
    encode_gps_query(gps_eQuery::eQUERY_REFERENCE_DATA, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendQueryReferenceParameters()
{
    encode_gps_query(gps_eQuery::eQUERY_REFERENCE_PARAMETERS, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendReferenceParameters(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec)
{
    encode_set_reference_parameters(integration_time_sec, max_integration_time_sec, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendCalcReference()
{
    encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_START, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendStopReference()
{
    encode_gps_reference_command(gps_eReferenceCommand::eReferenceCmd_ABORT, mBuffer);
    sendData();
}


