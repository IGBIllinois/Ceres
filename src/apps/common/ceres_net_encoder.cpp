

#include "ceres_net_encoder.hpp"
#include "packet_utils.hpp"

using namespace ceres;


cCeresNetEncoder::cCeresNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}


void cCeresNetEncoder::sendExperimentInfo(const std::string& title, const std::string& researcher,
    const std::string& cultivar, const std::string& doc)
{
    encode_exp_info_data(title, researcher, cultivar, doc, mBuffer);
    sendData();
    clear();
}

void cCeresNetEncoder::sendSpidercamPosition(const spidercam::sPosition& pos)
{
    encode_spidercam_pos(pos, mBuffer);
    sendData();
    clear();
}

void cCeresNetEncoder::sendWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    encode_weather_data(valid, wind_speed_mps, wind_direction_deg, mBuffer);
    sendData();
    clear();
}


