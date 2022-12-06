/*
   Packet Utilities of HySpex SWIR 384 Property Page <-> HySpex SWIR 384 controller
*/

#pragma once

#include "hyspex_swir_384_properties.pb.h"

#include <cstdint>

class net_buffer;
class net_buffer_view;

namespace hyspex
{
	namespace swir384
	{

		/**********************************************************
		   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

		   Only add new one to the end of the list of subsection!
		**********************************************************/
		enum class ePacketType : uint16_t
		{
			UNKNOWN = 0,

			QUERY_STATE = 1,

			CURRENT_STATE = 1000
		};


		/**********************************************************
		 * Property Page/Controller packets utilities
		 **********************************************************/

		int encode_query_state(net_buffer& buffer);

		struct sCurrentState_t
		{
			bool valid;
		};
		sCurrentState_t to_current_state_1(const CurrentState_1& pckt);
		int encode_current_state(bool valid, net_buffer& buffer);

	} // End of namespace vnir3000N

} // End of namespace hyspex


