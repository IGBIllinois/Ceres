

#include "HySpexCamera_ControllerNetDecoder.hpp"

#include "hyspex_packet_utils.hpp"
#include "net_buffer.hpp"

#include <QDebug>

//#define LOG_MESSAGE

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
#ifdef LOG_MESSAGE
            qInfo() << "Query status received.";
#endif
            onQueryState();
            break;
        case eQUERY_LENS_NAMES:
#ifdef LOG_MESSAGE
            qInfo() << "Query lens names received.";
#endif
            onQueryLensNames();
            break;
        case eQUERY_SHUTTER_STATE:
#ifdef LOG_MESSAGE
            qInfo() << "Query shutter state received.";
#endif
            onQueryShutterState();
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
    case ePacketType::SET_SHUTTER_STATE:
    {
        auto state = to_set_shutter_state_1(hdr.length, buffer);

        if (state == hyspex_eShutterState::eShutterState_OPEN)
            onOpenShutter();

        if (state == hyspex_eShutterState::eShutterState_CLOSED)
            onCloseShutter();

        break;
    }
    case ePacketType::HYSPEX_COMMAND:
    {
        auto command = to_hyspex_command_enum_1(hdr.length, buffer);
        switch (command)
        {
        case eCOMMAND_CALC_BACKGROUND:
            onCalcBackground();
            break;
        case eCOMMAND_STOP_BACKGROUND:
            onStopBackground();
            break;
        default:
            qWarning() << "Unknown command received: " << command;
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




