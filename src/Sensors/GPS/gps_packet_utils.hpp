/*
   Packet Utilities of GPS Property Page <-> GPS controller
*/

#pragma once

#include "gps_properties.pb.h"

#include <cstdint>
#include <string>

class sPacketHeader_t;
class net_buffer;
class net_buffer_view;

namespace gps
{
	/**********************************************************
	   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

	   Only add new one to the end of the list of subsection!
	**********************************************************/
	enum class ePacketType : uint16_t
	{
		UNKNOWN = 0,

		// Property Page -> Controller
		GPS_QUERY = 1,
		SET_REFERENCE_PARAMETERS = 2,
		GPS_REFERENCE_COMMAND = 3,

		// Controller -> Property Page
		REFERENCE_PARAMETERS = 1000,
		REFERENCE_DATA = 1001,
		REFERENCE_REPLY = 1002,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	/*** send/receive the query message ***/
	gps_eQuery to_gps_query_enum_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_gps_query(gps_eQuery query, net_buffer& buffer);

	/*** send/receive the reference command message ***/
	gps_eReferenceCommand to_gps_reference_command_enum_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_gps_reference_command(gps_eReferenceCommand command, net_buffer& buffer);

	/*** send/receive the set reference acquisition parameters message ***/
	struct sReferenceParameters_t
	{
		std::uint16_t min_integration_time_sec = 0;
		std::uint16_t max_integration_time_sec = 0;
		std::uint16_t ref_error_threshold_mm = 0;
	};
	sReferenceParameters_t to_reference_parameters_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_reference_parameters_set(std::uint16_t min_integration_time_sec, 
		std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm, net_buffer& buffer);
	int encode_reference_parameters_reply(std::uint16_t min_integration_time_sec,
		std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm, net_buffer& buffer);

	/*** send/receive  the reference position message ***/
	struct sReferenceData_t
	{
		bool valid = false;
		double avg_lat_rad = 0;
		double avg_lng_rad = 0;
		double avg_height_m = 0;
		double std_lat_rad = 0;
		double std_lng_rad = 0;
		double std_height_m = 0;
		bool   height_valid = false;
	};
	sReferenceData_t to_reference_data_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_reference_data(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
		double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid, net_buffer& buffer);

	/*** send/receive the reference command reply message ***/
	gps_eReferenceReply to_reference_reply_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_reference_reply(gps_eReferenceReply reply, net_buffer& buffer);

} // End of namespace hyspex

