

#include "HySpexCamera_ControllerNetDecoder.hpp"

#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"


void cHySpexCamera_ControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace hyspex;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    default:
    {
        break;
    }
    case ePacketType::QUERY_STATE:
    {
        onQueryState();
        break;
    }
    case ePacketType::QUERY_LENS_NAMES:
    {
        onQueryLensNames();
        break;
    }
    case ePacketType::SET_AVERAGE_FRAMES:
    {
        hyspex_SetAverageFrames_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_average_frames_1(packet);
        onSetAverageFrames(data);
        break;
    }
    case ePacketType::SET_FRAME_PERIOD_US:
    {
        hyspex_SetFramePeriod_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_frame_period_1(packet);
        onSetFramePeriod_us(data);
        break;
    }
    case ePacketType::SET_INTEGRATION_TIME_US:
    {
        hyspex_SetIntegrationTime_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_integration_time_1(packet);
        onSetIntegrationTime_us(data);
        break;
    }
    case ePacketType::SET_LENS_NAME:
    {
        hyspex_SetLens_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_lens_name_1(packet);
        onSetLensName(data);
        break;
    }
    case ePacketType::SET_NUM_BACKGROUNDS:
    {
        hyspex_SetNumOfBackgrounds_1 packet;
        packet.ParseFromArray(buffer.data(), hdr.length);
        auto data = to_num_backgrounds_1(packet);
        onSetNumOfBackgrounds(data);
        break;
    }
    case ePacketType::CALC_BACKGROUND:
    {
        onCalcBackground();
        break;
    }
    }
}




