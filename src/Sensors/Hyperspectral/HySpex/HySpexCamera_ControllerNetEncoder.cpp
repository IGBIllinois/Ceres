

#include "HySpexCamera_ControllerNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"


using namespace hyspex;

cHySpexCamera_ControllerNetEncoder::cHySpexCamera_ControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cHySpexCamera_ControllerNetEncoder::sendCurrentState(bool valid, std::uint16_t average_frames,
    std::uint32_t frame_period_us, std::uint32_t min_frame_period_us,
    std::uint32_t integration_time_us, std::uint32_t max_integration_time_us,
    std::uint32_t num_backgrounds, const std::string& lens_name)
{
    sCurrentState_t data;

    data.valid = valid;
    data.average_frames = average_frames;
    data.frame_period_us = frame_period_us;
    data.min_frame_period_us = min_frame_period_us;
    data.integration_time_us = integration_time_us;
    data.max_integration_time_us = max_integration_time_us;
    data.num_backgrounds = num_backgrounds;
    data.lens_name = lens_name;

    encode_current_state(data, mBuffer);
    sendData();
}

void cHySpexCamera_ControllerNetEncoder::sendLensNames(const std::vector<std::string>& names)
{
    encode_lens_names(names, mBuffer);
    sendData();
}
