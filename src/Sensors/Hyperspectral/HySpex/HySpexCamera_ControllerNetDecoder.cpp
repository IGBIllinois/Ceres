

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
    {
        break;
    }
    case ePacketType::HYSPEX_QUERY:
    {
        auto query = to_hyspex_query_enum_1(hdr.length, buffer);
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
        auto data = to_acquisition_parameters_1(hdr.length, buffer);
        onSetAcquisitionParameters(data.average_frames, data.frame_period_us, data.integration_time_us);
        break;
    }
    case ePacketType::SET_LENS_NAME:
    {
        auto data = to_lens_name_1(hdr.length, buffer);
        onSetLensName(data);
        break;
    }
    case ePacketType::SET_NUM_BACKGROUNDS:
    {
        auto data = to_num_backgrounds_1(hdr.length, buffer);
        onSetNumOfBackgrounds(data);
        break;
    }
    case ePacketType::CALC_BACKGROUND:
    {
        onCalcBackground();
        break;
    }
    default:
    {
        processPacket(static_cast<ePacketType>(hdr.id), hdr.length, buffer);
        break;
    }
    }
}




