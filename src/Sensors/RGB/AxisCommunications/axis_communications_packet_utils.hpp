/*
   Packet Utilities of Axis Communications Property Page <-> Axis Communications controller
*/

#pragma once

#include "axis_communications_properties.pb.h"

#include <cstdint>

class net_buffer;
class net_buffer_view;


namespace axis
{

	/**********************************************************
	   WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

	   Only add new one to the end of the list of subsection!
	**********************************************************/
	enum class ePacketType : uint16_t
	{
		UNKNOWN = 0,

		// Property Page <-> Controller
		CAMERA_MODE = 1,
		ACTIVE_CAMERA_ID,
		IMAGE_SIZE,
		FRAMES_PER_SECOND,
		LAPSE_INTERVAL_MS,

		// Property Page -> Controller
		QUERY_STATE = 1000,
		TAKE_PHOTO,
		GRAB_IMAGE,

		// Controller -> Property Page
		CURRENT_STATE = 2000,
		TAKE_PHOTO_REPLY,

		// Experiment State -> Controller
		SAVE_STATE = 3000,
		RESTORE_STATE,
	};


	/**********************************************************
	 * Property Page/Controller packets utilities
	 **********************************************************/

	int encode_query_current_state(net_buffer& buffer);
	int encode_query_camera_mode(net_buffer& buffer);
	int encode_query_active_camera_id(net_buffer& buffer);
	int encode_query_image_size(net_buffer& buffer);
	int encode_query_frame_rate(net_buffer& buffer);
	int encode_query_lapse_interval(net_buffer& buffer);
	int encode_grab_image(net_buffer& buffer);

	uint8_t to_active_camera_id_t(const axis_ActiveCameraIdMessage_1& pckt);
	int encode_active_camera_id(uint8_t id, net_buffer& buffer);

	uint8_t to_camera_mode_t(const axis_CameraModeMessage_1& pckt);
	int encode_camera_mode(uint8_t mode, net_buffer& buffer);


	struct sImageSize
	{
		uint16_t width;
		uint16_t height;
	};
	sImageSize to_image_size_t(const axis_ImageSizeMessage_1& pckt);
	int encode_image_size(uint16_t width, uint16_t height, net_buffer& buffer);

	uint8_t to_frame_rate_t(const axis_FrameRateMessage_1& pckt);
	int encode_frame_rate(uint8_t fps, net_buffer& buffer);

	uint32_t to_lapse_interval_t(const axis_LapseIntervalMessage_1& pckt);
	int encode_lapse_interval(uint32_t interval_ms, net_buffer& buffer);


	struct sCurrentState
	{
		bool     valid;
		uint8_t  mode;
		uint8_t  active_camera_id;
		uint16_t width;
		uint16_t height;
		uint8_t  frames_per_second;
		uint32_t lapse_interval_ms;
		uint8_t  min_camera_id;
		uint8_t  max_camera_id;
		std::optional<double> min_frames_per_second;
		std::optional<double> max_frames_per_second;
	};

	sCurrentState to_current_state_t(const axis_StateMessage_1& pckt);
	int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
		uint16_t height, uint8_t fps, net_buffer& buffer);

	sCurrentState to_current_state_t(const axis_StateMessage_2& pckt);
	int encode_current_state(bool valid, uint8_t camera_id, uint16_t width,
		uint16_t height, uint8_t fps, uint8_t min_camera_id, uint8_t max_camera_id, net_buffer& buffer);

	sCurrentState to_current_state_t(const axis_StateMessage_3& pckt);
	int encode_current_state(bool valid, uint8_t mode, uint8_t camera_id, uint16_t width,
		uint16_t height, uint8_t fps, uint32_t interval_ms, uint8_t min_camera_id, uint8_t max_camera_id,
		std::optional<double> min_fps, std::optional<double> max_fps, net_buffer& buffer);


	bool to_take_photo_t(const axis_TakePhoto_1& pckt);
	int encode_take_photo(bool update_view, net_buffer& buffer);

	struct sTakePhoto
	{
		bool update_view;
		bool auto_save;
	};
	sTakePhoto to_take_photo_t(const axis_TakePhoto_2& pckt);
	int encode_take_photo(bool update_view, bool auto_save, net_buffer& buffer);


	enum eReply { GOOD, FAILED, ABORTED, PENDING };
	eReply to_reply_t(const axis_Reply_1& pckt);
	//	int encode_camera_mode_reply(eReply reply, net_buffer& buffer);
	//	int encode_image_size_reply(eReply reply, net_buffer& buffer);
	///	int encode_frame_rate_reply(eReply reply, net_buffer& buffer);
	//	int encode_frame_interval_reply(eReply reply, net_buffer& buffer);
	//	int encode_thermal_range_reply(eReply reply, net_buffer& buffer);
	int encode_take_photo_reply(eReply reply, net_buffer& buffer);


	/**********************************************************
	 * Experiment State -> Controller packets utilities
	 **********************************************************/
	int encode_save_state(net_buffer& buffer);
	int encode_restore_state(net_buffer& buffer);
}
