
#include "ouster_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


int encode_query_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}


sAzimuthWindow_t to_azimuth_window_t(const SetAzimuthWindow_1& pckt)
{
    sAzimuthWindow_t data;

    data.min_deg = pckt.min_deg();
    data.max_deg = pckt.max_deg();

    return data;
}

int encode_azimuth_window(double min_deg, double max_deg, net_buffer& buffer)
{
    SetAzimuthWindow_1 pckt;

    pckt.set_min_deg(min_deg);
    pckt.set_max_deg(max_deg);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_AZIMUTH_WINDOW);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_lidar_mode_1(const SetLidarMode_1& pckt)
{
    return pckt.mode();
}

int encode_lidar_mode(const std::string& mode, net_buffer& buffer)
{
    SetLidarMode_1 pckt;

    pckt.set_mode(mode);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_LIDAR_MODE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sCurrentState_t to_current_state_1(const CurrentState_1& pckt)
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
    CurrentState_1 pckt;

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


int encode_query_lidar_modes(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_LIDAR_MODES);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}





