/*
   Packet Utilities of HySpex VNIR 3000N Property Page <-> HySpex VNIR 3000N controller
*/

#pragma once

#include "hyspex_properties.pb.h"

#include <cstdint>
#include <string>

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
		QUERY_STATE = 1,
		QUERY_LENS_NAMES = 2,
		SET_AVERAGE_FRAMES = 3,
		SET_FRAME_PERIOD_US = 4,
		SET_INTEGRATION_TIME_US = 5,
		SET_LENS_NAME = 6,
		SET_NUM_BACKGROUNDS = 7,
		CALC_BACKGROUND = 8,

		// Controller -> Property Page
		CURRENT_STATE = 1000,
		LENS_NAMES = 1001,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	int encode_query_state(net_buffer& buffer);
	int encode_query_lens_names(net_buffer& buffer);

	std::uint32_t to_average_frames_1(const hyspex_SetAverageFrames_1& pckt);
	int encode_average_frames(std::uint32_t average_frame, net_buffer& buffer);

	std::uint32_t to_frame_period_1(const hyspex_SetFramePeriod_1& pckt);
	int encode_frame_period(std::uint32_t frame_period_us, net_buffer& buffer);

	std::uint32_t to_integration_time_1(const hyspex_SetIntegrationTime_1& pckt);
	int encode_integration_time(std::uint32_t integration_time_us, net_buffer& buffer);

	std::string to_lens_name_1(const hyspex_SetLens_1& pckt);
	int encode_lens_name(const std::string& lens_name, net_buffer& buffer);

	int to_num_backgrounds_1(const hyspex_SetNumOfBackgrounds_1& pckt);
	int encode_num_backgrounds(int num_backgrounds, net_buffer& buffer);

	int encode_calc_background(net_buffer& buffer);

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
	sCurrentState_t to_current_state_1(const hyspex_CurrentState_1& pckt);
	int encode_current_state(const sCurrentState_t& state, net_buffer& buffer);

	std::vector<std::string> to_lens_names_1(const hyspex_LensNames_1& pckt);
	int encode_lens_names(const std::vector<std::string>& names, net_buffer& buffer);

} // End of namespace hyspex

