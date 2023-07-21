

#include "HySpexCamera_PropertiesNetDecoder.hpp"
#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"


void cHySpexCamera_PropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace hyspex;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::CURRENT_STATE:
    {
        hyspex_CurrentState_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        sCurrentState_t data = to_current_state_1(packet);
        
        onCurrentState(data.valid, data.average_frames,data.frame_period_us,
            data.min_frame_period_us, data.integration_time_us, data.max_integration_time_us,
            data.num_backgrounds, data.lens_name);

        break;
    }
    case ePacketType::LENS_NAMES:
    {
        hyspex_LensNames_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        onLensNames( to_lens_names_1(packet) );

        break;
    }
    }
}
