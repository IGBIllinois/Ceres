

#include "HySpexCamera_PropertiesNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"

using namespace hyspex;


cHySpexCamera_PropertiesNetEncoder::cHySpexCamera_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexCamera_PropertiesNetEncoder::sendQueryState()
{
    encode_hyspex_query(eQUERY_STATE, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendQueryLensNames()
{
    encode_hyspex_query(eQUERY_LENS_NAMES, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendQueryShutterState()
{
    encode_hyspex_query(eQUERY_SHUTTER_STATE, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendAcquisitionParameters(std::uint16_t average_frame,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    encode_acquisition_parameters(average_frame, frame_period_us, integration_time_us, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendLensName(const std::string& lens_name)
{
    encode_lens_name(lens_name, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendNumOfBackgrounds(int num_backgrounds)
{
    encode_num_backgrounds(num_backgrounds, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendCalcBackground()
{
    encode_calc_background(mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendOpenShutter()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_OPEN, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendCloseShutter()
{
    encode_set_shutter_state(hyspex_eShutterState::eShutterState_CLOSED, mBuffer);
    sendData();
}


