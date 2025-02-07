

#include "GpsControllerNetDecoder.hpp"

#include "gps_packet_utils.hpp"
#include "net_buffer.hpp"

#include <QDebug>

//#define LOG_MESSAGE

void cGpsControllerNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace gps;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    {
        break;
    }
    case ePacketType::GPS_QUERY:
    {
        auto query = to_gps_query_enum_1(hdr.length, buffer);
        switch (query)
        {
        case eQUERY_REFERENCE_DATA:
#ifdef LOG_MESSAGE
            qInfo() << "Query reference data received.";
#endif
            onQueryReferenceData();
            break;
        case eQUERY_REFERENCE_PARAMETERS:
#ifdef LOG_MESSAGE
            qInfo() << "Query reference parameters received.";
#endif
            onQueryReferenceParameters();
            break;
        case eQUERY_REFERENCE_STATE:
#ifdef LOG_MESSAGE
            qInfo() << "Query reference state received.";
#endif
            onQueryReferenceState();
            break;
        default:
            qWarning() << "Unknown query state received: " << query;
        }
        break;
    }
    case ePacketType::SET_REFERENCE_PARAMETERS:
    {
        auto data = to_reference_parameters_1(hdr.length, buffer);
        onSetReferenceParameters(data.min_integration_time_sec, data.max_integration_time_sec, data.ref_error_threshold_mm);
        break;
    }
    case ePacketType::GPS_REFERENCE_COMMAND:
    {
        auto command = to_gps_reference_command_enum_1(hdr.length, buffer);
        switch (command)
        {
        case eReferenceCmd_START:
            onCalcReference();
            break;
        case eReferenceCmd_ABORT:
            onStopReference();
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




