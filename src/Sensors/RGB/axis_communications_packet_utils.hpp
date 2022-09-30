/*
   Packet Utilities of Axis Communications Property Page <-> Axis Communications controller
*/

#pragma once

#include "axis_communications_properties.pb.h"

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

	QUERY_STATE = 1,
	SET_ACTIVE_CAMERA_ID,
	SET_RESOLUTION,
	SET_FRAMES_PER_SECOND,

	CURRENT_STATE
};



/**********************************************************
 * Property Page/Controller packets utilities
 **********************************************************/

int encode_query_state(net_buffer& buffer);

uint8_t to_active_camera_id_t(const SetActiveCameraId_1& pckt);
int encode_active_camera_id(uint8_t id, net_buffer& buffer);



/*
struct sCurrentState_t
{
	bool valid;
	std::string mode;
	double azimuth_min_deg;
	double azimuth_max_deg;
};
sCurrentState_t to_current_state_1(const CurrentState_1& pckt);
int encode_current_state(bool valid, const std::string& mode,
	double azimuth_min_deg, double azimuth_max_deg, net_buffer& buffer);
*/


