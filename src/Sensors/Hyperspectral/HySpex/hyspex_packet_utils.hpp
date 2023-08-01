/*
   Packet Utilities of HySpex Camera Property Page <-> HySpex Camera controller
*/

#pragma once

#include "hyspex_properties.pb.h"

#include <cstdint>
#include <string>

class sPacketHeader_t;
class net_buffer;
class net_buffer_view;

namespace hyspex
{
	/**********************************************************
	   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

	   Only add new one to the end of the list of subsection!
	**********************************************************/
	enum class ePacketType : uint16_t
	{
		UNKNOWN = 0,

		// Property Page -> Controller
		HYSPEX_QUERY = 1,
		SET_ACQUISITION_PARAMETERS = 2,
		SET_LENS_NAME = 3,
		SET_NUM_BACKGROUNDS = 4,
		CALC_BACKGROUND = 5,
		SET_SHUTTER_STATE = 6,

		// Controller -> Property Page
		CURRENT_STATE = 1000,
		LENS_NAMES = 1001,
		BACKGROUND_REPLY = 1002,
		SHUTTER_STATE_REPLY = 1003,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	/*** send/receive the query message ***/
	hyspex_eQuery to_hyspex_query_enum_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_hyspex_query(hyspex_eQuery query, net_buffer& buffer);

	/*** send/receive the acquisition parameters message ***/
	struct sAcquisitionParameters_t
	{
		std::uint16_t average_frames = 0;
		std::uint32_t frame_period_us = 0;
		std::uint32_t integration_time_us = 0;
	};
	sAcquisitionParameters_t to_acquisition_parameters_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_acquisition_parameters(std::uint16_t average_frame,
		std::uint32_t frame_period_us, std::uint32_t integration_time_us, net_buffer& buffer);

	/*** send/receive the lens name message ***/
	std::string to_lens_name_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_lens_name(const std::string& lens_name, net_buffer& buffer);

	/*** send/receive the number of backgrounds message ***/
	int to_num_backgrounds_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_num_backgrounds(int num_backgrounds, net_buffer& buffer);

	/*** send the calculate backgrounds message ***/
	int encode_calc_background(net_buffer& buffer);

	/*** send/receive the current state message ***/
	struct sCurrentState_t
	{
		bool valid = false;
		std::uint16_t average_frames = 0;
		std::uint32_t frame_period_us = 0;
		std::uint32_t min_frame_period_us = 0;
		std::uint32_t integration_time_us = 0;
		std::uint32_t max_integration_time_us = 0;
		std::uint32_t num_backgrounds = 0;
		std::string lens_name;
	};
	sCurrentState_t to_current_state_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_current_state(const sCurrentState_t& state, net_buffer& buffer);

	/*** send/receive the list of lens names message ***/
	std::vector<std::string> to_lens_names_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_lens_names(const std::vector<std::string>& names, net_buffer& buffer);

	/*** send/receive the background reply message ***/
	hyspex_eBackgroundReply to_background_reply_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_background_reply(hyspex_eBackgroundReply reply, net_buffer& buffer);

	/*** send/receive the shutter state set/reply message ***/
	hyspex_eShutterState to_set_shutter_state_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_set_shutter_state(hyspex_eShutterState state, net_buffer& buffer);

	hyspex_eShutterState to_shutter_state_reply_1(std::uint16_t length, const net_buffer_view& buffer);
	int encode_shutter_state_reply(hyspex_eShutterState state, net_buffer& buffer);

} // End of namespace hyspex

