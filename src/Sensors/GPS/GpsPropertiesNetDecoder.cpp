

#include "GpsPropertiesNetDecoder.hpp"
#include "gps_packet_utils.hpp"
#include "net_buffer.hpp"


void cGpsPropertiesNetDecoder::processPacket(const sPacketHeader_t& hdr, const net_buffer_view& buffer)
{
    using namespace gps;

    switch (static_cast<ePacketType>(hdr.id))
    {
    case ePacketType::UNKNOWN:
    {
        break;
    }
    case ePacketType::REFERENCE_PARAMETERS:
    {
        sReferenceParameters_t data = to_reference_parameters_1(hdr.length, buffer);

        onReferenceParameters(true, data.min_integration_time_sec, data.max_integration_time_sec, data.ref_error_threshold_mm);

        break;
    }
    case ePacketType::REFERENCE_DATA:
    {
        sReferenceData_t data = to_reference_data_1(hdr.length, buffer);
        
        onReferenceData(data.valid, data.avg_lat_rad, data.avg_lng_rad, data.avg_height_m,
            data.std_lat_rad, data.std_lng_rad, data.std_height_m, data.height_valid);

        break;
    }
    case ePacketType::REFERENCE_REPLY:
    {
        auto reply = to_reference_reply_1(hdr.length, buffer);
        switch (reply)
        {
        case gps_eReferenceReply::eReferenceReply_GOOD:
            onReferenceCommandReply(eReferenceReply::GOOD);
            break;
        case gps_eReferenceReply::eReferenceReply_ABORTED:
            onReferenceCommandReply(eReferenceReply::ABORTED);
            break;
        case gps_eReferenceReply::eReferenceReply_FAILED:
            onReferenceCommandReply(eReferenceReply::FAILED);
            break;
        case gps_eReferenceReply::eReferenceReply_PENDING:
            onReferenceCommandReply(eReferenceReply::PENDING);
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


