/*
   Packet Utilities of OUSTER Property Page <-> OUSTER controller
*/

#pragma once

#include "ouster_properties.pb.h"

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
	SET_AZIMUTH_WINDOW,
	SET_LIDAR_MODE,

	CURRENT_STATE
};



/**********************************************************
 * Property Page/Controller packets utilities
 **********************************************************/

int encode_query_state(net_buffer& buffer);

struct sAzimuthWindow_t
{
	double min_deg;
	double max_deg;
};
sAzimuthWindow_t to_azimuth_window_t(const SetAzimuthWindow_1& pckt);
int encode_azimuth_window(double min_deg, double max_deg, net_buffer& buffer);

std::string to_lidar_mode_1(const SetLidarMode_1& pckt);
int encode_lidar_mode(const std::string& mode, net_buffer& buffer);

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



