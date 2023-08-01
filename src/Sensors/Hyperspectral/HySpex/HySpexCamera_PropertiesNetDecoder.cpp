

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
        
        onCurrentState(data.valid, data.average_frames,data.frame_period_us,
            data.min_frame_period_us, data.integration_time_us, data.max_integration_time_us,
            data.num_backgrounds, data.lens_name);

        break;
    }
    case ePacketType::LENS_NAMES:
    {
        onLensNames( to_lens_names_1(hdr.length, buffer) );

        break;
    }
    case ePacketType::BACKGROUND_REPLY:
    {
        auto reply = to_background_reply_1(hdr.length, buffer);
        switch (reply)
        {
        case hyspex_eBackgroundReply::eBackgroundReply_GOOD:
            onBackgroundReply(eBackgroundReply::GOOD);
            break;
        case hyspex_eBackgroundReply::eBackgroundReply_ABORTED:
            onBackgroundReply(eBackgroundReply::ABORTED);
            break;
        case hyspex_eBackgroundReply::eBackgroundReply_FAILED:
            onBackgroundReply(eBackgroundReply::FAILED);
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
            onShutterState(eShutterState::UNKNOWN);
            break;
        case hyspex_eShutterState::eShutterState_OPEN:
            onShutterState(eShutterState::OPEN);
            break;
        case hyspex_eShutterState::eShutterState_CLOSED:
            onShutterState(eShutterState::CLOSED);
            break;
        case hyspex_eShutterState::eShutterState_PENDING_OPEN:
            onShutterState(eShutterState::PENDING_OPEN);
            break;
        case hyspex_eShutterState::eShutterState_PENDING_CLOSE:
            onShutterState(eShutterState::PENDING_CLOSED);
            break;
        case hyspex_eShutterState::eShutterState_ERROR:
            onShutterState(eShutterState::ERROR);
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
