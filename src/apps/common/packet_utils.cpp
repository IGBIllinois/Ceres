
#include "packet_utils.hpp"
#include "net_buffer.hpp"

using namespace ceres;

#include <string>

#if defined(_WIN32)

#include <WinSock2.h>
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    namespace sc = std::chrono;
    sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
    sc::seconds s = sc::duration_cast<sc::seconds>(d);
    tp->tv_sec = s.count();
    tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

    return 0;
}

#else
#include <sys/time.h>
#endif // _WIN32


namespace
{
    void set_timestamp(sPacketHeader_t::sTimestamp* timestamp)
    {
        struct timeval tv;

        gettimeofday(&tv, nullptr);

        timestamp->seconds = tv.tv_sec;
        timestamp->nanos = (tv.tv_usec * 1000);
    }
}



/*
 * Ceres <---> Ceres Remote Client Packets
 */
sExperimentInfo_t to_experiment_info_1(const ExperimentInfo_1& pckt)
{
    sExperimentInfo_t data;

    data.title      = pckt.title();
    data.researcher = pckt.researcher();
    data.cultivar   = pckt.cultivar();
    data.doc        = pckt.file();

    return data;
}

int encode_exp_info_data(const std::string& title, const std::string& researcher,
    const std::string& cultivar, const std::string& doc, ceres::net_buffer& buffer)
{
    ExperimentInfo_1 pckt;

    pckt.set_title(title);

    if (!researcher.empty())
        pckt.set_researcher(researcher);

    if (!cultivar.empty())
        pckt.set_researcher(cultivar);

    pckt.set_file(doc);


    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = ePacketType::EXPERIMENT_INFO_1;
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


/*
 * Spidercam Packets
 */
spidercam::sPosition to_spidercam_position_1(const Spidercam_Position_1& pckt)
{
    spidercam::sPosition data;

    data.X_mm = pckt.x_mm();
    data.Y_mm = pckt.y_mm();
    data.Z_mm = pckt.z_mm();
    data.speed_mmps = pckt.speed_mmps();
    data.pan_deg = pckt.pan_deg();
    data.pitch_deg = pckt.pitch_deg();
    data.roll_gimbal_deg = pckt.roll_deg();
    data.timestamp = pckt.timestamp();

    data.focus = 0;
    data.height_mm = data.Z_mm;
    data.iris = 0;
    data.pan_speed_dps = 0.0;
    data.roll_deg = data.roll_gimbal_deg;
    data.tilt_deg = 0;
    data.tilt_speed_dps = 0;

    return data;
}

int encode_spidercam_pos(const spidercam::sPosition& pos, net_buffer& buffer)
{
    Spidercam_Position_1 pckt;

    pckt.set_datavalid(true);
    pckt.set_x_mm(pos.X_mm);
    pckt.set_y_mm(pos.Y_mm);
    pckt.set_z_mm(pos.Z_mm);
    pckt.set_speed_mmps(pos.speed_mmps);
    pckt.set_pan_deg(pos.pan_deg);
    pckt.set_pitch_deg(pos.pitch_deg);
    pckt.set_roll_deg(pos.roll_gimbal_deg);
    pckt.set_timestamp(pos.timestamp);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = ePacketType::SPIDER_CAM_DATA_1;
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


/*
 * Weather Packets
 */
sWeatherData_t to_weather_data_1(const WeatherData_1& pckt)
{
    sWeatherData_t data;

    data.dataValid = pckt.datavalid();
    data.wind_speed_mps = pckt.wind_speed_mps();
    data.wind_direction_deg = pckt.wind_direction_deg();

    return data;
}

int encode_weather_data(bool valid, double wind_speed_mps, double wind_direction_deg, net_buffer& buffer)
{
    WeatherData_1 pckt;

    pckt.set_datavalid(valid);
    pckt.set_wind_speed_mps(wind_speed_mps);
    pckt.set_wind_direction_deg(wind_direction_deg);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = ePacketType::WEATHER_DATA_1;
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


net_buffer& operator>>(net_buffer& buffer, sPacketHeader_t& hdr)
{
    uint16_t id;
    buffer >> id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    hdr.id = static_cast<ePacketType>(id);
    return buffer;
}

net_buffer& operator<<(net_buffer& buffer, const sPacketHeader_t& hdr)
{
    buffer << static_cast<uint16_t>(hdr.id) << hdr.revision << hdr.length << hdr.timestamp.seconds << hdr.timestamp.nanos;
    return buffer;
}

net_buffer_view& operator>>(net_buffer_view& buffer, sPacketHeader_t& hdr)
{
    uint16_t id;
    buffer >> id >> hdr.revision >> hdr.length >> hdr.timestamp.seconds >> hdr.timestamp.nanos;
    hdr.id = static_cast<ePacketType>(id);
    return buffer;
}
