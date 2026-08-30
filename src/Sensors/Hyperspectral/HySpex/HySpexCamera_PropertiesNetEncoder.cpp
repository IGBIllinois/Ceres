

#include "HySpexCamera_PropertiesNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"

using namespace hyspex;


cHySpexCamera_PropertiesNetEncoder::cHySpexCamera_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/**   Encode Methods   */
void cHySpexCamera_PropertiesNetEncoder::encodeQueryStateMessage()
{
    if (encode_hyspex_query(eQUERY_STATE, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_STATE, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeQueryLensNamesMessage()
{
    if (encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeQueryShutterStateMessage()
{
    if (encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    if (encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer) < 0)
    {
        sendData();
        encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeLensNameMessage(const std::string& lens_name)
{
    if (encode_lens_name(lens_name, mBuffer) < 0)
    {
        sendData();
        encode_lens_name(lens_name, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeNumOfBackgroundsMessage(int num_backgrounds)
{
    if (encode_num_backgrounds(num_backgrounds, mBuffer) < 0)
    {
        sendData();
        encode_num_backgrounds(num_backgrounds, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeCalcBackgroundMessage()
{
    if (encode_hyspex_command(eCOMMAND_CALC_BACKGROUND,  mBuffer) < 0)
    {
        sendData();
        encode_hyspex_command(eCOMMAND_CALC_BACKGROUND, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeStopBackgroundMessage()
{
    if (encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer) < 0)
    {
        sendData();
        encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeOpenShutterMessage()
{
    if (encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer) < 0)
    {
        sendData();
        encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer);
    }
}

void cHySpexCamera_PropertiesNetEncoder::encodeCloseShutterMessage()
{
    if (encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer) < 0)
    {
        sendData();
        encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer);
    }
}

/**   Send Methods   */
void cHySpexCamera_PropertiesNetEncoder::sendQueryStateMessage()
{
    encode_hyspex_query(eQUERY_STATE, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendQueryLensNamesMessage()
{
    encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendQueryShutterStateMessage()
{
    encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendAcquisitionParametersMessage(std::uint16_t average_frame,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendLensNameMessage(const std::string& lens_name)
{
    encode_lens_name(lens_name, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendNumOfBackgroundsMessage(int num_backgrounds)
{
    encode_num_backgrounds(num_backgrounds, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendCalcBackgroundMessage()
{
    encode_hyspex_command(eCOMMAND_CALC_BACKGROUND, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendStopBackgroundMessage()
{
    encode_hyspex_command(eCOMMAND_STOP_BACKGROUND, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendOpenShutterMessage()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendCloseShutterMessage()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer);
    sendData();
}


