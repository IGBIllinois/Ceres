/*
   Packet Utilities of HySpex VNIR 3000N Property Page <-> HySpex VNIR 3000N controller
*/

#pragma once

#include "hyspex_properties.pb.h"

#include <cstdint>

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

		QUERY_STATE = 1,

		CURRENT_STATE = 1000,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	int encode_query_state(net_buffer& buffer);

	struct sCurrentState_t
	{
		bool valid = false;
		std::uint16_t avgerage_frames = 0;
		std::uint32_t frame_period_us = 0;
		std::uint32_t min_frame_period_us = 0;
		std::uint32_t integration_time_us = 0;
		std::uint32_t max_integration_time_us = 0;
		std::uint32_t num_backgrounds = 0;
		std::string lens_name;
	};
	sCurrentState_t to_current_state_1(const hyspex_CurrentState_1& pckt);
	int encode_current_state(const sCurrentState_t& state, net_buffer& buffer);


	namespace vnir3000N
	{
		/**********************************************************
		   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

		   Only add new one to the end of the list of subsection!
		**********************************************************/
		enum class ePacketType : uint16_t
		{
			UNKNOWN = 0,

			QUERY_STATE = 1,

			CURRENT_STATE = 1000,
		};


	} // End of namespace vnir3000N

} // End of namespace hyspex

