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

	QUERY_STATE,
	ACTIVE_CAMERA_ID,
	IMAGE_SIZE,
	FRAMES_PER_SECOND,

	CURRENT_STATE
};


/**********************************************************
 * Property Page/Controller packets utilities
 **********************************************************/

int encode_query_current_state(net_buffer& buffer);
int encode_query_active_camera_id(net_buffer& buffer);
int encode_query_image_size(net_buffer& buffer);
int encode_query_frame_rate(net_buffer& buffer);

uint8_t to_active_camera_id_t(const axis_ActiveCameraIdMessage_1& pckt);
int encode_active_camera_id(uint8_t id, net_buffer& buffer);


struct sImageSize
{
	uint16_t width;
	uint16_t height;
};
sImageSize to_image_size_t(const axis_ImageSizeMessage_1& pckt);
int encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer);


uint8_t to_frame_rate_t(const axis_FrameRateMessage_1& pckt);
int encode_frame_rate(uint8_t fps, net_buffer& buffer);


struct sCurrentState
{
	bool     valid;
	uint8_t  camera_id;
	uint16_t width;
	uint16_t height;
	uint8_t  frames_per_second;
};
sCurrentState to_current_state_t(const axis_StateMessage_1& pckt);
int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
	uint16_t height, uint8_t fps, net_buffer& buffer);


