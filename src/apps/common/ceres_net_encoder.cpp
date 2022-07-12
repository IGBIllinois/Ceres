

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
}

void cCeresNetEncoder::sendStartExperiment()
{
    encode_start_experiment(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStopExperiment()
{
    encode_stop_experiment(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendOpenDataFile(const std::string& fileName)
{
    encode_open_data_file(fileName, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendCloseDataFile()
{
    encode_close_data_file(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStartDataRecording()
{
    encode_start_data_recording(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStopDataRecording()
{
    encode_stop_data_recording(mBuffer);
    sendData();
}


void cCeresNetEncoder::sendSpidercamPosition(const spidercam::sPosition_1_t& pos)
{
    encode_spidercam_pos(pos, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendWeatherData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    encode_weather_data(valid, wind_speed_mps, wind_direction_deg, mBuffer);
    sendData();
}


