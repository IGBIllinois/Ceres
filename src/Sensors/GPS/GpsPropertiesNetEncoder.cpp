

#include "GpsPropertiesNetEncoder.hpp"
#include "gps_packet_utils.hpp"

using namespace gps;


cGpsPropertiesNetEncoder::cGpsPropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{
}

/**   Encode Methods   */
void cGpsPropertiesNetEncoder::encodeQueryState()
{
    if (encode_hyspex_query(eQUERY_STATE, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_STATE, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeQueryLensNames()
{
    if (encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeQueryShutterState()
{
    if (encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    if (encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer) < 0)
    {
        sendData();
        encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeLensName(const std::string& lens_name)
{
    if (encode_lens_name(lens_name, mBuffer) < 0)
    {
        sendData();
        encode_lens_name(lens_name, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeNumOfBackgrounds(int num_backgrounds)
{
    if (encode_num_backgrounds(num_backgrounds, mBuffer) < 0)
    {
        sendData();
        encode_num_backgrounds(num_backgrounds, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeCalcBackground()
{
    if (encode_hyspex_command(eCOMMAND_CALC_BACKGROUND, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_command(eCOMMAND_CALC_BACKGROUND, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeStopBackground()
{
    if (encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeOpenShutter()
{
    if (encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer) < 0)
    {
        sendData();
        encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer);
    }
}

void cGpsPropertiesNetEncoder::encodeCloseShutter()
{
    if (encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer) < 0)
    {
        sendData();
        encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer);
    }
}

/**   Send Methods   */
void cGpsPropertiesNetEncoder::sendQueryState()
{
    encode_hyspex_query(eQUERY_STATE, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendQueryLensNames()
{
    encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendQueryShutterState()
{
    encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendAcquisitionParameters(std::uint16_t average_frame,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendLensName(const std::string& lens_name)
{
    encode_lens_name(lens_name, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendNumOfBackgrounds(int num_backgrounds)
{
    encode_num_backgrounds(num_backgrounds, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendCalcBackground()
{
    encode_hyspex_command(eCOMMAND_CALC_BACKGROUND, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendStopBackground()
{
    encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendOpenShutter()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer);
    sendData();
}

void cGpsPropertiesNetEncoder::sendCloseShutter()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer);
    sendData();
}


