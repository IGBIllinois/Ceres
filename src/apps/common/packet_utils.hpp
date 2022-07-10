/*
   GNSS Receiver Communication SDK for C++ and Asio
   For connecting to and communicating with a Septentrio GNSS receiver, using C++/Asio
*/

#pragma once

#include "ceres_remote_client.pb.h"
#include "spidercam_data.pb.h"
#include "weather_data.pb.h"

#include <spidercam/spidercam_types.hpp>
#include <cstdint>

namespace ceres
{
	class net_buffer;
	class net_buffer_view;
}


/**********************************************************
   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

   Only add new one to the end of the list of subsection!
**********************************************************/
enum class ePacketType : uint16_t
{
	UNKNOWN = 0,

	EXPERIMENT_INFO = 1,
	EXPERIMENT_INFO_REPLY,
	OPEN_DATA_FILE,
	CLOSE_DATA_FILE,
	DATA_FILE_STATE,
	START_DATA_RECORDING,
	START_DATA_RECORDING_REPLY,
	STOP_DATA_RECORDING,
	STOP_DATA_RECORDING_REPLY,
	END_DATA_RECORDING,

	SPIDER_CAM_DATA = 1000,

	WEATHER_DATA	= 1100,
};

struct sPacketHeader_t 
{
   ePacketType id;
   uint8_t revision;
   uint16_t length;

   struct sTimestamp
   {
	   uint64_t seconds;
	   uint32_t nanos;
   }
   timestamp;
};


/**********************************************************
 * Ceres/Ceres Remote Client packets utilities
 **********************************************************/
struct sExperimentInfo_t
{
	std::string title;
	std::string researcher;
	std::string cultivar;
	std::string doc;
};

sExperimentInfo_t to_experiment_info_1(const ExperimentInfo_1& pckt);
int encode_exp_info_data(const std::string& title, const std::string& researcher,
	const std::string& cultivar, const std::string& doc, ceres::net_buffer& buffer);

std::string to_filename_1(const OpenDataFile_1& pckt);
int encode_open_data_file(const std::string& filename, ceres::net_buffer& buffer);

int encode_close_data_file(ceres::net_buffer& buffer);

bool to_file_open_state_1(const FileOpenState_1& pckt);
int encode_file_open_state(bool open, ceres::net_buffer& buffer);


/**********************************************************
 * Spidercam packets utilities
 **********************************************************/
spidercam::sPosition to_spidercam_position_1(const Spidercam_Position_1& pckt);
int encode_spidercam_pos(const spidercam::sPosition& pos, ceres::net_buffer& buffer);


/**********************************************************
 * Weather Data packets utilities
 **********************************************************/
struct sWeatherData_t
{
	bool  dataValid;
	float wind_speed_mps;
	float wind_direction_deg;
};

sWeatherData_t to_weather_data_1(const WeatherData_1& pckt);
int encode_weather_data(bool valid, double wind_speed_mps, double wind_direction_deg, ceres::net_buffer& buffer);


ceres::net_buffer& operator>>(ceres::net_buffer& buffer, sPacketHeader_t& hdr);
ceres::net_buffer_view& operator>>(ceres::net_buffer_view& buffer, sPacketHeader_t& hdr);
ceres::net_buffer& operator<<(ceres::net_buffer& buffer, const sPacketHeader_t& hdr);

