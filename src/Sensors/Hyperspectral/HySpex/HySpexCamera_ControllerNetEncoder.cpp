

#include "HySpexCamera_ControllerNetEncoder.hpp"
#include "hyspex_packet_utils.hpp"
//#include "hyspex_properties.pb.h"


using namespace hyspex;

cHySpexCamera_ControllerNetEncoder::cHySpexCamera_ControllerNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

/**   Encode Methods   */
void cHySpexCamera_ControllerNetEncoder::encodeCurrentStateMessage(bool valid, std::uint16_t average_frames,
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

void cHySpexCamera_ControllerNetEncoder::encodeLensNamesMessage(const std::vector<std::string>& names)
{
    if (encode_lens_names(names, mBuffer) < 0)
    {
        sendData();
        encode_lens_names(names, mBuffer);
    }
}

void cHySpexCamera_ControllerNetEncoder::encodeLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg)
{
    if (encode_lens_info(name, working_distance_cm, fov_deg, mBuffer) < 0)
    {
        sendData();
        encode_lens_info(name, working_distance_cm, fov_deg, mBuffer);
    }
}

void cHySpexCamera_ControllerNetEncoder::encodeCommandReplyMessage(hyspex_eCommand reply)
{
    if (encode_command_reply(reply, mBuffer) < 0)
    {
        sendData();
        encode_command_reply(reply, mBuffer);
    }
}

void cHySpexCamera_ControllerNetEncoder::encodeBackgroundReplyMessage(hyspex_eBackgroundReply reply)
{
    if (encode_background_reply(reply, mBuffer) < 0)
    {
        sendData();
        encode_background_reply(reply, mBuffer);
    }
}

void cHySpexCamera_ControllerNetEncoder::encodeShutterStateReplyMessage(hyspex_eShutterState state)
{
    if (encode_shutter_state_reply(state, mBuffer) < 0)
    {
        sendData();
        encode_shutter_state_reply(state, mBuffer);
    }
}


/**   Send Methods   */
void cHySpexCamera_ControllerNetEncoder::sendCurrentStateMessage(bool valid, std::uint16_t average_frames,
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

void cHySpexCamera_ControllerNetEncoder::sendLensNamesMessage(const std::vector<std::string>& names)
{
    encode_lens_names(names, mBuffer);
    sendData();
}

void cHySpexCamera_ControllerNetEncoder::sendLensInfoMessage(const std::string& name, double working_distance_cm, double fov_deg)
{
    encode_lens_info(name, working_distance_cm, fov_deg, mBuffer);
    sendData();
}

void cHySpexCamera_ControllerNetEncoder::sendCommandReplyMessage(hyspex_eCommand reply)
{
    encode_command_reply(reply, mBuffer);
    sendData();
}

void cHySpexCamera_ControllerNetEncoder::sendBackgroundReplyMessage(hyspex_eBackgroundReply reply)
{
    encode_background_reply(reply, mBuffer);
    sendData();
}

void cHySpexCamera_ControllerNetEncoder::sendShutterStateReplyMessage(hyspex_eShutterState state)
{
    encode_shutter_state_reply(state, mBuffer);
    sendData();
}
