

#include "HySpexCamera_ControllerNetDecoder.hpp"

#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"

#include <QDebug>

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
    case ePacketType::HYSPEX_QUERY:
    {
        hyspex_QueryMessage_1 pckt;
        pckt.ParseFromArray(buffer.data(), hdr.length);
        auto query = pckt.query();
//        auto query = to_hyspex_query_enum_1(hdr, buffer);
        switch (query)
        {
        case eQUERY_STATE:
            qInfo() << "Query status received.";
            onQueryState();
            break;
        case eQUERY_LENS_NAMES:
            qInfo() << "Query lens names received.";
            onQueryLensNames();
            break;
        default:
            qWarning() << "Unknown query state received: " << query;
        }
        break;
    }
    case ePacketType::SET_ACQUISITION_PARAMETERS:
    {
//        hyspex_SetAcquisitionParameters_1 packet;
//        packet.ParseFromArray(buffer.data(), hdr.length);
//        auto data = to_acquisition_parameters_1(packet);
        auto data = to_acquisition_parameters_1(hdr, buffer);
        onSetAcquisitionParameters(data.average_frames, data.frame_period_us, data.integration_time_us);
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




