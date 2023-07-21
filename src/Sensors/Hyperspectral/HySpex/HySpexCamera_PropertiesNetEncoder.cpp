

#include "HySpexCamera_PropertiesNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"

using namespace hyspex;


cHySpexCamera_PropertiesNetEncoder::cHySpexCamera_PropertiesNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexCamera_PropertiesNetEncoder::sendQueryState()
{
    encode_query_state(mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendQueryLensNames()
{
    encode_query_lens_names(mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendAverageFrames(std::uint32_t average_frame)
{
    encode_average_frames(average_frame, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendFramePeriod_us(std::uint32_t frame_period_us)
{
    encode_frame_period(frame_period_us, mBuffer);
    sendData();
}

void cHySpexCamera_PropertiesNetEncoder::sendIntegrationTime_us(std::uint32_t integration_time_us)
{
    encode_integration_time(integration_time_us, mBuffer);
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


