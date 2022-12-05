
#include "ouster_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


int encode_query(ouster_eQuery query, net_buffer& buffer)
{
    ouster_QueryMessage_1 pckt;

    pckt.set_query(query);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_query_state(net_buffer& buffer)
{
    return encode_query(eQUERY_STATE, buffer);
}

int encode_query_lidar_mode(net_buffer& buffer)
{
    return encode_query(eQUERY_LIDAR_MODE, buffer);
}

int encode_query_azimuth_window(net_buffer& buffer)
{
    return encode_query(eQUERY_AZIMUTH_WINDOW, buffer);
}


sAzimuthWindow_t to_azimuth_window_t(const ouster_AzimuthWindowMessage_1& pckt)
{
    sAzimuthWindow_t data;

    data.min_deg = pckt.min_deg();
    data.max_deg = pckt.max_deg();

    return data;
}

int encode_azimuth_window(double min_deg, double max_deg, net_buffer& buffer)
{
    ouster_AzimuthWindowMessage_1 pckt;

    pckt.set_min_deg(min_deg);
    pckt.set_max_deg(max_deg);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::AZIMUTH_WINDOW);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_lidar_mode_1(const ouster_LidarModeMessage_1& pckt)
{
    return pckt.mode();
}

int encode_lidar_mode(const std::string& mode, net_buffer& buffer)
{
    ouster_LidarModeMessage_1 pckt;

    pckt.set_mode(mode);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::LIDAR_MODE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sCurrentState_t to_current_state_1(const ouster_StateMessage_1& pckt)
{
    sCurrentState_t data;

    data.valid = pckt.valid();

    if (data.valid)
    {
        data.mode = pckt.mode();
        data.azimuth_min_deg = pckt.azimuth_min_deg();
        data.azimuth_max_deg = pckt.azimuth_max_deg();
    }
    else
    {
        data.azimuth_min_deg = -180.0;
        data.azimuth_max_deg = -180.0;
    }

    return data;
}

int encode_current_state(bool valid, const std::string& mode,
    double azimuth_min_deg, double azimuth_max_deg, net_buffer& buffer)
{
    ouster_StateMessage_1 pckt;

    pckt.set_valid(valid);
    pckt.set_mode(mode);
    pckt.set_azimuth_min_deg(azimuth_min_deg);
    pckt.set_azimuth_max_deg(azimuth_max_deg);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


//int encode_query_lidar_modes(net_buffer& buffer)
//{
//    sPacketHeader_t hdr;
//    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_LIDAR_MODES);
//    hdr.revision = 1;
//    hdr.length = 0;
//    set_timestamp(&hdr.timestamp);

//    buffer << hdr;

//    return sizeof(sPacketHeader_t);
//}





