/*
   Packet Utilities of Teledyne FLIR Property Page <-> Teledyne FLIR controller
*/

#pragma once

#include "teledyne_flir_properties.pb.h"

#include <cstdint>
#include <optional>

class net_buffer;
class net_buffer_view;

namespace flir
{
	/**********************************************************
	   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

	   Only add new one to the end of the list of subsection!
	**********************************************************/
	enum class ePacketType : uint16_t
	{
		UNKNOWN = 0,

		QUERY_STATE,
		CAMERA_MODE,
		IMAGE_SIZE,
		FRAMES_PER_SECOND,
		FRAMES_INTERVAL_MS,

		CURRENT_STATE,
		GRAB_IMAGE,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	int encode_query_current_state(net_buffer& buffer);
	int encode_query_camera_mode(net_buffer& buffer);
	int encode_query_image_size(net_buffer& buffer);
	int encode_query_frame_rate(net_buffer& buffer);
	int encode_query_frame_interval(net_buffer& buffer);
	int encode_grab_image(net_buffer& buffer);

	uint8_t to_camera_mode_t(const teledyne_CameraModeMessage_1& pckt);
	int encode_camera_mode(uint8_t mode, net_buffer& buffer);


	struct sImageSize
	{
		uint16_t width;
		uint16_t height;
	};
	sImageSize to_image_size_t(const teledyne_ImageSizeMessage_1& pckt);
	int encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer);


	double to_frame_rate_t(const teledyne_FrameRateMessage_1& pckt);
	int encode_frame_rate(double fps, net_buffer& buffer);


	uint32_t to_frame_interval_t(const teledyne_FrameIntervalMessage_1& pckt);
	int encode_frame_interval(uint32_t interval_ms, net_buffer& buffer);


	struct sCurrentState
	{
		bool     valid;
		uint8_t  mode;
		uint16_t width;
		uint16_t height;
		double   frames_per_second;
		uint32_t frames_interval_ms;
		std::optional<double> min_frames_per_second;
		std::optional<double> max_frames_per_second;
	};
	sCurrentState to_current_state_t(const teledyne_StateMessage_1& pckt);
	int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
		uint16_t height, double fps, uint32_t interval_ms, 
		std::optional<double> min_fps, std::optional<double> max_fps, net_buffer& buffer);
}
