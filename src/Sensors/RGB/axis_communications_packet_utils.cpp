
#include "axis_communications_packet_utils.hpp"
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


uint8_t to_active_camera_id_t(const SetActiveCameraId_1& pckt)
{
    return static_cast<uint8_t>(pckt.camera_id());
}

int encode_active_camera_id(uint8_t id, net_buffer& buffer)
{
    SetActiveCameraId_1 pckt;

    pckt.set_camera_id(id);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_ACTIVE_CAMERA_ID);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

/*
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
*/




