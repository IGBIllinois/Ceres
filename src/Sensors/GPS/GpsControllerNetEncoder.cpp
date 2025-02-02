

#include "GpsControllerNetEncoder.hpp"
#include "gps_packet_utils.hpp"


using namespace gps;

cGpsControllerNetEncoder::cGpsControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/**   Encode Methods   */
void cGpsControllerNetEncoder::encodeCurrentState(bool valid, std::uint16_t average_frames,
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

    if (encode_current_state(data, mBuffer) < 0)
    {
        sendData();
        encode_current_state(data, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeLensNames(const std::vector<std::string>& names)
{
    if (encode_lens_names(names, mBuffer) < 0)
    {
        sendData();
        encode_lens_names(names, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeCommandReply(hyspex_eCommand reply)
{
    if (encode_command_reply(reply, mBuffer) < 0)
    {
        sendData();
        encode_command_reply(reply, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeBackgroundReply(hyspex_eBackgroundReply reply)
{
    if (encode_background_reply(reply, mBuffer) < 0)
    {
        sendData();
        encode_background_reply(reply, mBuffer);
    }
}

void cGpsControllerNetEncoder::encodeShutterStateReply(hyspex_eShutterState state)
{
    if (encode_shutter_state_reply(state, mBuffer) < 0)
    {
        sendData();
        encode_shutter_state_reply(state, mBuffer);
    }
}


/**   Send Methods   */
void cGpsControllerNetEncoder::sendCurrentState(bool valid, std::uint16_t average_frames,
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

void cGpsControllerNetEncoder::sendLensNames(const std::vector<std::string>& names)
{
    encode_lens_names(names, mBuffer);
    sendData();
}

void cGpsControllerNetEncoder::sendCommandReply(hyspex_eCommand reply)
{
    encode_command_reply(reply, mBuffer);
    sendData();
}

void cGpsControllerNetEncoder::sendBackgroundReply(hyspex_eBackgroundReply reply)
{
    encode_background_reply(reply, mBuffer);
    sendData();
}

void cGpsControllerNetEncoder::sendShutterStateReply(hyspex_eShutterState state)
{
    encode_shutter_state_reply(state, mBuffer);
    sendData();
}
