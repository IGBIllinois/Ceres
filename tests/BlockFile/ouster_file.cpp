

#include <catch2/catch_all.hpp>

#include "BlockDataFile.hpp"
#include "OusterSerializer.hpp"
#include "OusterParser.hpp"

#include <ouster/ouster_defs.h>
#include <ouster/ouster_utils.h>
#include <ouster/OusterLidarData.h>


TEST_CASE("Lidar Sonsor Info tests", "[initialization]")
{

	SECTION("Testing read/write of sensor info data...")
	{
		using namespace ouster;
		sensor_info_2_t original_data;
		original_data.product_line = "OS-0-128";
		original_data.product_part_number = "840-102144-C";
		original_data.product_serial_number = "992037000167";
		original_data.image_rev = "ousteros-image-prod-aries-v2.3.0+20220415163956";
		original_data.build_revision.major = 2;
		original_data.build_revision.minor = 3;
		original_data.build_revision.patch = 0;
		original_data.build_date = "2022-04-14T21:11:47Z";
		original_data.status = eSENSOR_STATUS::INITIALIZING;
		// "initialization_id" : 2573185;
		original_data.base_part_number = "";
		original_data.base_serial_number = "";

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto sensorInfo = ouster.getSensorInfo_2();

			REQUIRE(sensorInfo.product_line == original_data.product_line);
			REQUIRE(sensorInfo.product_part_number == original_data.product_part_number);
			REQUIRE(sensorInfo.product_serial_number == original_data.product_serial_number);
			REQUIRE(sensorInfo.image_rev == original_data.image_rev);
			REQUIRE(sensorInfo.build_revision.major == original_data.build_revision.major);
			REQUIRE(sensorInfo.build_revision.minor == original_data.build_revision.minor);
			REQUIRE(sensorInfo.build_revision.patch == original_data.build_revision.patch);
			REQUIRE(sensorInfo.build_date == original_data.build_date);
			REQUIRE(sensorInfo.status == original_data.status);
			REQUIRE(sensorInfo.base_part_number == original_data.base_part_number);
			REQUIRE(sensorInfo.base_serial_number == original_data.base_serial_number);

			rd.close();
		}
	}

	SECTION("Testing read/write of IMU intrinsics data...")
	{
		using namespace ouster;
		imu_intrinsics_2_t original_data;
		original_data.imu_to_sensor_transform.resize(16);
		original_data.imu_to_sensor_transform[0] = 1;
		original_data.imu_to_sensor_transform[1] = 0;
		original_data.imu_to_sensor_transform[2] = 0;
		original_data.imu_to_sensor_transform[3] = 6.253;
		original_data.imu_to_sensor_transform[4] = 0;
		original_data.imu_to_sensor_transform[5] = 1;
		original_data.imu_to_sensor_transform[6] = 0;
		original_data.imu_to_sensor_transform[7] = -11.775;
		original_data.imu_to_sensor_transform[8] = 0;
		original_data.imu_to_sensor_transform[9] = 0;
		original_data.imu_to_sensor_transform[10] = 1;
		original_data.imu_to_sensor_transform[11] = 7.645;
		original_data.imu_to_sensor_transform[12] = 0;
		original_data.imu_to_sensor_transform[13] = 0;
		original_data.imu_to_sensor_transform[14] = 0;
		original_data.imu_to_sensor_transform[15] = 1;

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto imuInfo = ouster.getImuIntrinsics_2();

			REQUIRE(imuInfo.imu_to_sensor_transform.size() == original_data.imu_to_sensor_transform.size());

			REQUIRE(imuInfo.imu_to_sensor_transform == original_data.imu_to_sensor_transform);

			rd.close();
		}
	}


	SECTION("Testing read/write of beam intrinsics data...")
	{
		using namespace ouster;
		beam_intrinsics_2_t original_data;
		original_data.altitude_angles_deg.resize(128);
		original_data.altitude_angles_deg = { 46.25, 45.24, 44.54, 44.12,
			43.24, 42.27, 41.58, 41.16, 40.28, 39.31, 38.64, 38.21, 37.33,
			36.4, 35.71, 35.27, 34.38, 33.47, 32.81, 32.36, 31.47, 30.59,
			29.93, 29.46, 28.56, 27.71, 27.06, 26.58, 25.68, 24.84, 24.19,
			23.71, 22.81, 21.99, 21.35, 20.85, 19.94, 19.17, 18.52, 18, 17.09,
			16.33, 15.71, 15.16, 14.26, 13.53, 12.9, 12.34, 11.43, 10.73,
			10.09, 9.52, 8.619999999999999, 7.93, 7.3, 6.72, 5.81, 5.14, 4.51,
			3.9, 3, 2.35, 1.72, 1.09, 0.19, -0.43, -1.07, -1.72, -2.63, -3.24,
			-3.86, -4.54, -5.44, -6.04, -6.66, -7.36, -8.27, -8.84, -9.48,
			-10.19, -11.1, -11.65, -12.29, -13.02, -13.93, -14.47, -15.11, -15.88,
			-16.78, -17.3, -17.95, -18.73, -19.64, -20.15, -20.8, -21.61, -22.51,
			-23, -23.66, -24.5, -25.39, -25.88, -26.54, -27.4, -28.29, -28.77, -29.43,
			-30.31, -31.2, -31.67, -32.34, -33.25, -34.13, -34.58, -35.27, -36.21,
			-37.09, -37.53, -38.22, -39.19, -40.08, -40.51, -41.21, -42.21, -43.1,
			-43.53, -44.24, -45.26 };

		original_data.azimuth_angles_deg.resize(128);
		original_data.azimuth_angles_deg = { 11.59, 4.26, -2.95, -10.05, 11.09,
			4.06, -2.88, -9.73, 10.69, 3.89, -2.82, -9.470000000000001, 10.33,
			3.73, -2.78, -9.23, 10.01, 3.61, -2.74, -9.029999999999999, 9.74, 3.49,
			-2.7, -8.859999999999999, 9.5, 3.39, -2.68, -8.720000000000001,
			9.300000000000001, 3.29, -2.67, -8.6, 9.109999999999999, 3.2, -2.66, -8.51,
			8.949999999999999, 3.14, -2.66, -8.44, 8.82, 3.06, -2.66,
			-8.369999999999999, 8.69, 3.01, -2.65, -8.33, 8.609999999999999, 2.95,
			-2.68, -8.32, 8.52, 2.9, -2.7, -8.31, 8.460000000000001, 2.87, -2.72,
			-8.32, 8.41, 2.82, -2.75, -8.35, 8.380000000000001, 2.79, -2.78,
			-8.380000000000001, 8.35, 2.76, -2.81, -8.44, 8.34, 2.75, -2.87, -8.5,
			8.35, 2.72, -2.9, -8.58, 8.369999999999999, 2.7, -2.97, -8.68, 8.4, 2.71,
			-3.03, -8.81, 8.44, 2.69, -3.11, -8.94, 8.51, 2.68, -3.18, -9.1, 8.59, 2.67,
			-3.27, -9.279999999999999, 8.69, 2.68, -3.34, -9.49, 8.800000000000001, 2.69,
			-3.46, -9.720000000000001, 8.949999999999999, 2.71, -3.59, -10,
			9.119999999999999, 2.74, -3.73, -10.31, 9.34, 2.76, -3.89, -10.68, 9.58, 2.79,
			-4.07, -11.1, 9.880000000000001, 2.85, -4.28, -11.61 };

		original_data.lidar_to_beam_origins_mm = 27.67;


		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(4096, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto beamInfo = ouster.getBeamIntrinsics_2();

			REQUIRE(beamInfo.altitude_angles_deg == original_data.altitude_angles_deg);
			REQUIRE(beamInfo.azimuth_angles_deg == original_data.azimuth_angles_deg);
			REQUIRE(beamInfo.lidar_to_beam_origins_mm == original_data.lidar_to_beam_origins_mm);

			rd.close();
		}
	}


	SECTION("Testing read/write of lidar intrinsics data...")
	{
		using namespace ouster;
		lidar_intrinsics_2_t original_data;

		original_data.lidar_to_sensor_transform = { -1, 0, 0, 0, 0, -1, 0, 0,
			0, 0, 1, 36.18, 0, 0, 0, 1 };

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto lidarInfo = ouster.getLidarIntrinsics_2();

			REQUIRE(lidarInfo.lidar_to_sensor_transform == original_data.lidar_to_sensor_transform);

			rd.close();
		}
	}


	SECTION("Testing read/write of time info data...")
	{
		using namespace ouster;
		time_info_2_t original_data;

		original_data.timestamp_info.time = 4080.49558297;
		original_data.timestamp_info.mode = to_timestamp_mode("TIME_FROM_INTERNAL_OSC");
		original_data.timestamp_info.ptp_1588 = 4087;
		original_data.timestamp_info.sync_pulse_in = 0;
		original_data.timestamp_info.internal_osc = 4080;

		original_data.sync_pulse_info.count = 0;
		original_data.sync_pulse_info.count_unfiltered = 1;
		original_data.sync_pulse_info.last_period_nsec = 0;
		original_data.sync_pulse_info.locked = 0;
		original_data.sync_pulse_info.polarity = to_polarity("ACTIVE_HIGH");

		original_data.multipurpose_io_info.angle_deg = 360;
		original_data.multipurpose_io_info.frequency_hz = 1;
		original_data.multipurpose_io_info.mode = to_io_pin_mode("OFF");
		original_data.multipurpose_io_info.polarity = to_polarity("ACTIVE_HIGH");
		original_data.multipurpose_io_info.pulse_width_ms = 10;

		original_data.nmea_info.baud_rate = to_baud_rate("BAUD_9600");
		original_data.nmea_info.bit_count = 1;
		original_data.nmea_info.bit_count_unfiltered = 0;
		original_data.nmea_info.char_count = 0;
		original_data.nmea_info.date_decoded_count = 0;
		original_data.nmea_info.ignore_valid_char = 0;
		original_data.nmea_info.last_read_message = "";
		original_data.nmea_info.leap_seconds = 0;
		original_data.nmea_info.locked = 0;
		original_data.nmea_info.not_valid_count = 0;
		original_data.nmea_info.polarity = to_polarity("ACTIVE_HIGH");
		original_data.nmea_info.start_char_count = 0;
		original_data.nmea_info.utc_decoded_count = 0;

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto timeInfo = ouster.getTimeInfo_2();

			REQUIRE(timeInfo.timestamp_info.time == original_data.timestamp_info.time);
			REQUIRE(timeInfo.timestamp_info.mode == original_data.timestamp_info.mode);
			REQUIRE(timeInfo.timestamp_info.ptp_1588 == original_data.timestamp_info.ptp_1588);
			REQUIRE(timeInfo.timestamp_info.sync_pulse_in == original_data.timestamp_info.sync_pulse_in);
			REQUIRE(timeInfo.timestamp_info.internal_osc == original_data.timestamp_info.internal_osc);

			REQUIRE(timeInfo.sync_pulse_info.count == original_data.sync_pulse_info.count);
			REQUIRE(timeInfo.sync_pulse_info.count_unfiltered == original_data.sync_pulse_info.count_unfiltered);
			REQUIRE(timeInfo.sync_pulse_info.last_period_nsec == original_data.sync_pulse_info.last_period_nsec);
			REQUIRE(timeInfo.sync_pulse_info.locked == original_data.sync_pulse_info.locked);
			REQUIRE(timeInfo.sync_pulse_info.polarity == original_data.sync_pulse_info.polarity);

			REQUIRE(timeInfo.multipurpose_io_info.angle_deg == original_data.multipurpose_io_info.angle_deg);
			REQUIRE(timeInfo.multipurpose_io_info.frequency_hz == original_data.multipurpose_io_info.frequency_hz);
			REQUIRE(timeInfo.multipurpose_io_info.mode == original_data.multipurpose_io_info.mode);
			REQUIRE(timeInfo.multipurpose_io_info.polarity == original_data.multipurpose_io_info.polarity);
			REQUIRE(timeInfo.multipurpose_io_info.pulse_width_ms == original_data.multipurpose_io_info.pulse_width_ms);

			REQUIRE(timeInfo.nmea_info.baud_rate == original_data.nmea_info.baud_rate);
			REQUIRE(timeInfo.nmea_info.bit_count == original_data.nmea_info.bit_count);
			REQUIRE(timeInfo.nmea_info.bit_count_unfiltered == original_data.nmea_info.bit_count_unfiltered);
			REQUIRE(timeInfo.nmea_info.char_count == original_data.nmea_info.char_count);
			REQUIRE(timeInfo.nmea_info.date_decoded_count == original_data.nmea_info.date_decoded_count);
			REQUIRE(timeInfo.nmea_info.ignore_valid_char == original_data.nmea_info.ignore_valid_char);
			REQUIRE(timeInfo.nmea_info.last_read_message == original_data.nmea_info.last_read_message);
			REQUIRE(timeInfo.nmea_info.leap_seconds == original_data.nmea_info.leap_seconds);
			REQUIRE(timeInfo.nmea_info.locked == original_data.nmea_info.locked);
			REQUIRE(timeInfo.nmea_info.not_valid_count == original_data.nmea_info.not_valid_count);
			REQUIRE(timeInfo.nmea_info.polarity == original_data.nmea_info.polarity);
			REQUIRE(timeInfo.nmea_info.start_char_count == original_data.nmea_info.start_char_count);
			REQUIRE(timeInfo.nmea_info.utc_decoded_count == original_data.nmea_info.utc_decoded_count);

			rd.close();
		}
	}


	SECTION("Testing read/write of lidar data format data...")
	{
		using namespace ouster;
		lidar_data_format_2_t original_data;

		original_data.pixels_per_column = 128;
		original_data.columns_per_packet = 16;
		original_data.columns_per_frame = 1024;
		original_data.pixel_shift_by_row = { 66, 45, 25, 4, 65, 45, 25, 5, 63,
			44, 25, 6, 62, 44, 25, 7, 61, 43, 25, 7, 61, 43, 25, 8, 60, 43,
			25, 8, 59, 42, 25, 9, 59, 42, 25, 9, 58, 42, 25, 9, 58, 42, 25,
			9, 58, 42, 25, 9, 57, 41, 25, 9, 57, 41, 25, 9, 57, 41, 25, 9, 57,
			41, 25, 9, 57, 41, 25, 9, 57, 41, 25, 9, 57, 41, 25, 9, 57, 41, 25,
			9, 57, 41, 25, 8, 57, 41, 24, 8, 57, 41, 24, 8, 57, 41, 24, 7, 57,
			41, 24, 7, 58, 41, 23, 6, 58, 41, 23, 5, 58, 41, 23, 5, 59, 41, 22,
			4, 60, 41, 22, 3, 60, 41, 21, 1, 61, 41, 21, 0 };

		original_data.column_window_min = 384;
		original_data.column_window_max = 640;
		original_data.udp_profile_lidar = "LEGACY";
		original_data.udp_profile_imu = "LEGACY";

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());

			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(original_data);
			wrt.close();
		}

		{
			cBlockDataFileReader rd;
			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto formatInfo = ouster.getLidarDataFormat_2();

			REQUIRE(formatInfo.pixels_per_column == original_data.pixels_per_column);
			REQUIRE(formatInfo.columns_per_packet == original_data.columns_per_packet);
			REQUIRE(formatInfo.columns_per_frame == original_data.columns_per_frame);
			REQUIRE(formatInfo.pixel_shift_by_row == original_data.pixel_shift_by_row);
			REQUIRE(formatInfo.column_window_min == original_data.column_window_min);
			REQUIRE(formatInfo.column_window_max == original_data.column_window_max);
			REQUIRE(formatInfo.udp_profile_lidar == original_data.udp_profile_lidar);
			REQUIRE(formatInfo.udp_profile_imu == original_data.udp_profile_imu);

			rd.close();
		}
	}
}

TEST_CASE("Lidar IMU/LiDAR Data tests", "[stream data]")
{

	SECTION("Testing write/read of lidar data...")
	{
		using namespace ouster;
		cOusterLidarData original_data;

		const uint16_t pixels_per_column = 5;
		const uint16_t columns_per_frame = 10;

		original_data.resize(pixels_per_column, columns_per_frame);

		lidar_data_block_t pixel;

		for (uint16_t c = 0; c < columns_per_frame; ++c)
		{
			pixel.range_mm = c + 1;
			pixel.intensity = c + 1;
			pixel.reflectivity = c + 1;
			pixel.ambient_noise = c + 1;

			for (uint16_t r = 0; r < pixels_per_column; ++r)
			{
				original_data.channel(c, r, pixel);
			}
		}

		{
			cBlockDataFileWriter wrt;
			wrt.open("test.lidar_data");

			REQUIRE(wrt.isOpen());
			cOusterSerializer ouster(1024, &wrt);
			ouster.setVersion(2, 3);

			ouster.write(1, original_data);

			wrt.close();
		}

		{
			cBlockDataFileReader rd;

			rd.open("test.lidar_data");

			REQUIRE(rd.isOpen());

			cOusterParser ouster;
			rd.attach(&ouster);

			auto result = rd.processBlock();
			REQUIRE(result);

			auto lidarData = ouster.getLidarData();
			REQUIRE(lidarData.columnsPerFrame() == columns_per_frame);
			REQUIRE(lidarData.pixelsPerColumn() == pixels_per_column);

			lidar_data_block_t original_pixel;
			lidar_data_block_t pixel;

			for (uint16_t col = 0; col < columns_per_frame; ++col)
			{
				for (uint16_t chn = 0; chn < pixels_per_column; ++chn)
				{
					original_pixel = original_data.channel(col, chn);
					pixel = lidarData.channel(col, chn);

					REQUIRE(original_pixel.ambient_noise == pixel.ambient_noise);
					REQUIRE(original_pixel.intensity == pixel.intensity);
					REQUIRE(original_pixel.range_mm == pixel.range_mm);
					REQUIRE(original_pixel.reflectivity == pixel.reflectivity);
				}
			}

			result = rd.processBlock();
			REQUIRE(!result);

			rd.close();
		}
	}
}

