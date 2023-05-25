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

class net_buffer;
class net_buffer_view;



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
	STOP_DATA_RECORDING,
	DATA_RECORDING_STATE,
	START_EXPERIMENT,
	STOP_EXPERIMENT,
	STATUS_MESSAGE,
	LOG_MESSAGE,
	SENSOR_STATUS,
	SENSOR_NAME_CHANGE,
	SENSOR_PROPERTY_CONNECT_INFO,
	TREATMENT,
	PRINCIPAL_INVESTIGATOR,
	CONSTRUCT_NAME,
	EVENT_NUMBER,
	FIELD_DESIGN,
	PLANTING_DATE,
	HARVEST_DATE,
	COMMENT,

	SPIDER_CAM_DATA = 1000,

	WEATHER_DATA	= 1100,
};



/**********************************************************
 * Ceres/Ceres Remote Client packets utilities
 **********************************************************/
struct sExperimentInfo_t
{
	std::string title;
	std::string researcher;
	std::string species;
	std::string cultivar;
	std::string doc;
};

sExperimentInfo_t to_experiment_info_1(const ExperimentInfo_1& pckt);
sExperimentInfo_t to_experiment_info_2(const ExperimentInfo_2& pckt);

int encode_exp_info_data(const std::string& title, const std::string& researcher,
	const std::string& cultivar, const std::string& doc, net_buffer& buffer);
int encode_exp_info_data(const std::string& title, const std::string& researcher,
	const std::string& species, const std::string& cultivar, const std::string& doc, net_buffer& buffer);

std::string to_principal_investigator_1(const PrincipalInvestigator_1& pckt);
int encode_principal_investigator(const std::string& pi, net_buffer& buffer);

std::string to_construct_name_1(const ConstructName_1& pckt);
int encode_construct_name(const std::string& name, net_buffer& buffer);

std::string to_event_number_1(const EventNumber_1& pckt);
int encode_event_number(const std::string& number, net_buffer& buffer);

std::string to_field_design_1(const FieldDesign_1& pckt);
int encode_field_design(const std::string& design, net_buffer& buffer);

std::time_t to_planting_date_1(const PlantingDate_1& pckt);
int encode_planting_date(std::time_t date, net_buffer& buffer);

std::time_t to_harvest_date_1(const HarvestDate_1& pckt);
int encode_harvest_date(std::time_t date, net_buffer& buffer);

std::string to_treatment_1(const ExperimentTreatment_1& pckt);
int encode_treatment(const std::string& treatment, net_buffer& buffer);

std::string to_comment_1(const ExperimentComment_1& pckt);
int encode_comment(const std::string& comment, net_buffer& buffer);


int encode_start_experiment(net_buffer& buffer);
int encode_stop_experiment(net_buffer& buffer);

std::string to_filename_1(const OpenDataFile_1& pckt);
int encode_open_data_file(const std::string& filename, net_buffer& buffer);

int encode_close_data_file(net_buffer& buffer);

bool to_file_open_state_1(const FileOpenState_1& pckt);
int encode_file_open_state(bool open, net_buffer& buffer);

int encode_start_data_recording(net_buffer& buffer);
int encode_stop_data_recording(net_buffer& buffer);

std::string to_status_message_1(const StatusMessage_1& pckt);
int encode_status_message(const std::string& message, net_buffer& buffer);

struct sLogMessage_t
{
	uint8_t  msg_type;
	std::string device;
	std::string message;
};
sLogMessage_t to_log_message_1(const LogMessage_1& pckt);
int encode_log_message(uint8_t msg_type, const std::string& device, const std::string& message, net_buffer& buffer);

struct sSensorStatus_t
{
	std::string name;
	std::string status;
};
sSensorStatus_t to_sensor_status_1(const SensorStatus_1& pckt);
int encode_sensor_status(const std::string& device, const std::string& message, net_buffer& buffer);

struct sSensorNameChange_t
{
	std::string old_name;
	std::string new_name;
};
sSensorNameChange_t to_sensor_name_change_1(const SensorNameChange_1& pckt);
int encode_sensor_name_change(const std::string& old_name, const std::string& new_name, net_buffer& buffer);

struct sSensorPropertyConnectInfo_t
{
	std::string sensor;
	std::string model;
	uint32_t    version;
	std::string name;
	std::string ip_address;
	uint16_t	port;
};
sSensorPropertyConnectInfo_t to_sensor_property_connect_info_1(const SensorPropertyConnectInfo_1& pckt);
int encode_sensor_property_connect_info(const std::string& sensor, 
	const std::string& model, uint32_t version,
	const std::string& name, const std::string& ip_address, uint16_t port, net_buffer& buffer);


/**********************************************************
 * Spidercam packets utilities
 **********************************************************/
spidercam::sPosition_1_t to_spidercam_position_1(const Spidercam_Position_1& pckt);
int encode_spidercam_pos(const spidercam::sPosition_1_t& pos, net_buffer& buffer);


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
int encode_weather_data(bool valid, double wind_speed_mps, double wind_direction_deg, net_buffer& buffer);


