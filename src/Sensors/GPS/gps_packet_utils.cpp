
#include "gps_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>

gps_eQuery gps::to_gps_query_enum_1(std::uint16_t length, const net_buffer_view& buffer)
{
    gps_QueryMessage_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.query();
    return gps_eQuery();
}

int gps::encode_gps_query(gps_eQuery query, net_buffer& buffer)
{
    gps_QueryMessage_1 pckt;
    pckt.set_query(query);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::GPS_QUERY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

gps_eReferenceCommand gps::to_gps_reference_command_enum_1(std::uint16_t length, const net_buffer_view& buffer)
{
    gps_ReferenceCommand_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.command();

    return gps_eReferenceCommand();
}

int gps::encode_gps_reference_command(gps_eReferenceCommand command, net_buffer& buffer)
{
    gps_ReferenceCommand_1 pckt;
    pckt.set_command(command);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::GPS_REFERENCE_COMMAND);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

gps::sReferenceParameters_t gps::to_reference_parameters_1(std::uint16_t length, const net_buffer_view& buffer)
{
    gps_SetReferenceParameters_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        gps::sReferenceParameters_t data;
        data.min_integration_time_sec = pckt.min_integration_time_sec();
        data.max_integration_time_sec = pckt.max_integration_time_sec();
        data.ref_error_threshold_mm = pckt.ref_error_threshold_mm();
        return data;
    }

    return gps::sReferenceParameters_t();
}

int gps::encode_reference_parameters_set(std::uint16_t integration_time_sec,
    std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm, net_buffer& buffer)
{
    gps_SetReferenceParameters_1 pckt;
    pckt.set_min_integration_time_sec(integration_time_sec);
    pckt.set_max_integration_time_sec(max_integration_time_sec);
    pckt.set_ref_error_threshold_mm(ref_error_threshold_mm);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_REFERENCE_PARAMETERS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int gps::encode_reference_parameters_reply(std::uint16_t min_integration_time_sec,
    std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm, net_buffer& buffer)
{
    gps_SetReferenceParameters_1 pckt;
    pckt.set_min_integration_time_sec(min_integration_time_sec);
    pckt.set_max_integration_time_sec(max_integration_time_sec);
    pckt.set_ref_error_threshold_mm(ref_error_threshold_mm);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::REFERENCE_PARAMETERS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

gps::sReferenceData_t gps::to_reference_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    gps_ReferenceData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        gps::sReferenceData_t data;
        data.valid = pckt.valid();
        data.avg_lat_rad = pckt.avg_lat_rad();
        data.avg_lng_rad = pckt.avg_lng_rad();
        data.avg_height_m = pckt.avg_height_m();
        data.std_lat_rad = pckt.std_lat_rad();
        data.std_lng_rad = pckt.std_lng_rad();
        data.std_height_m = pckt.std_height_m();
        data.height_valid = pckt.height_valid();
        return data;
    }

    return gps::sReferenceData_t();
}

int gps::encode_reference_data(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
    double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid, net_buffer& buffer)
{
    gps_ReferenceData_1 pckt;
    pckt.set_valid(valid);
    pckt.set_avg_lat_rad(avg_lat_rad);
    pckt.set_avg_lng_rad(avg_lng_rad);
    pckt.set_avg_height_m(avg_height_m);
    pckt.set_std_lat_rad(std_lat_rad);
    pckt.set_std_lng_rad(std_lng_rad);
    pckt.set_std_height_m(std_height_m);
    pckt.set_height_valid(height_valid);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::REFERENCE_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

/*** send/receive the command reply message ***/
gps_eReferenceReply gps::to_reference_reply_1(std::uint16_t length, const net_buffer_view& buffer)
{
    gps_RefCommandReply_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.reply();

    return gps_eReferenceReply();
}

int gps::encode_reference_reply(gps_eReferenceReply reply, net_buffer& buffer)
{
    gps_RefCommandReply_1 pckt;
    pckt.set_reply(reply);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::REFERENCE_REPLY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


