

#include "HySpexCamera_PropertiesNetDecoder.hpp"
#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"


void cHySpexCamera_PropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace hyspex;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    {
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        sCurrentState_t data = to_current_state_1(hdr.length, buffer);
        
        onCurrentStateMessage(data.valid, data.average_frames,data.frame_period_us,
            data.min_frame_period_us, data.integration_time_us, data.max_integration_time_us,
            data.num_backgrounds, data.lens_name);

        break;
    }
    case ePacketType::LENS_NAMES:
    {
        onLensNamesMessage( to_lens_names_1(hdr.length, buffer) );

        break;
    }
    case ePacketType::LENS_INFO:
    {
        auto info = to_lens_info_1(hdr.length, buffer);
        onLensInfoMessage(info.lens_name, info.working_distance_cm, info.fov_deg);
        break;
    }
    case ePacketType::COMMAND_REPLY:
    {
        auto reply = to_command_reply_1(hdr.length, buffer);
        switch (reply)
        {
        case hyspex_eCommand::eCOMMAND_CALC_BACKGROUND:
            onCommandReplyMessage(eCommandReply::CALC_BACKGROUND);
            break;
        case hyspex_eCommand::eCOMMAND_STOP_BACKGROUND:
            onCommandReplyMessage(eCommandReply::STOP_BACKGROUND);
            break;
        case hyspex_eCommand::eCOMMAND_UNSPECIFIED:
        default:
            onCommandReplyMessage(eCommandReply::UNKNOWN);
            break;
        }
        break;
    }
    case ePacketType::BACKGROUND_REPLY:
    {
        auto reply = to_background_reply_1(hdr.length, buffer);
        switch (reply)
        {
        case hyspex_eBackgroundReply::eBackgroundReply_GOOD:
            onBackgroundReplyMessage(eBackgroundReply::GOOD);
            break;
        case hyspex_eBackgroundReply::eBackgroundReply_ABORTED:
            onBackgroundReplyMessage(eBackgroundReply::ABORTED);
            break;
        case hyspex_eBackgroundReply::eBackgroundReply_FAILED:
            onBackgroundReplyMessage(eBackgroundReply::FAILED);
            break;
        case hyspex_eBackgroundReply::eBackgroundReply_PENDING:
            onBackgroundReplyMessage(eBackgroundReply::PENDING);
            break;
        }
        break;
    }
    case ePacketType::SHUTTER_STATE_REPLY:
    {
        auto reply = to_shutter_state_reply_1(hdr.length, buffer);
        switch (reply)
        {
        case hyspex_eShutterState::eShutterState_UNKNOWN:
            onShutterStateMessage(eShutterState::UNKNOWN);
            break;
        case hyspex_eShutterState::eShutterState_OPEN:
            onShutterStateMessage(eShutterState::OPEN);
            break;
        case hyspex_eShutterState::eShutterState_CLOSED:
            onShutterStateMessage(eShutterState::CLOSED);
            break;
        case hyspex_eShutterState::eShutterState_PENDING_OPEN:
            onShutterStateMessage(eShutterState::PENDING_OPEN);
            break;
        case hyspex_eShutterState::eShutterState_PENDING_CLOSE:
            onShutterStateMessage(eShutterState::PENDING_CLOSED);
            break;
        case hyspex_eShutterState::eShutterState_ERROR:
            onShutterStateMessage(eShutterState::ERROR);
            break;
        }
        break;
    }
    default:
    {
        processPacket(static_cast<ePacketType>(hdr.id), hdr.length, buffer);
        break;
    }
    }
}
