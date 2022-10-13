
#include "OusterParser.hpp"
#include "OusterDataIdentifiers.hpp"
#include "BlockDataFile.hpp"

#include <stdexcept>

using namespace ouster;

namespace
{
    ouster::version_t to_version(cDataBuffer& buffer)
    {
        std::string str;
        buffer >> str;

        ouster::version_t version = ouster::invalid_version;

        if (str[0] != 'v')
            return version;
        auto first = str.find_first_of('.');
        version.major = std::stoi(str.substr(1, first));
        auto next = str.find_first_of('.', first + 1);
        version.minor = std::stoi(str.substr(first + 1, next));
        std::size_t pos = 0;
        std::string s = str.substr(next + 1);
        version.patch = std::stoi(s, &pos);
        if (pos < s.length())
        {
            version.extra = s.substr(pos);
        }

        return version;
    }

    ouster::eOPERATING_MODE to_operating_mode(cDataBuffer& buffer)
    {
        uint8_t mode = 0;
        buffer >> mode;
        return static_cast<ouster::eOPERATING_MODE>(mode); 
    };

    ouster::eSENSOR_STATUS to_sensor_status(cDataBuffer& buffer)
    {
        uint8_t status = 0;
        buffer >> status;
        return static_cast<ouster::eSENSOR_STATUS>(status);
    };

    ouster::eLIDAR_MODE to_lidar_mode(cDataBuffer& buffer)
    {
        uint8_t mode = 0;
        buffer >> mode;
        return static_cast<ouster::eLIDAR_MODE>(mode);
    };

    ouster::eTIMESTAMP_MODE to_timestamp_mode(cDataBuffer& buffer)
    {
        uint8_t mode = 0;
        buffer >> mode;
        return static_cast<ouster::eTIMESTAMP_MODE>(mode);
    };

    ouster::eNMEA_BAUD_RATE to_nmea_baud_rate(cDataBuffer& buffer)
    {
        uint8_t rate = 0;
        buffer >> rate;
        return static_cast<ouster::eNMEA_BAUD_RATE>(rate);
    };

    ouster::ePOLARITY to_polarity(cDataBuffer& buffer)
    {
        uint8_t mode = 0;
        buffer >> mode;
        return static_cast<ouster::ePOLARITY>(mode);
    };

    ouster::eIO_PIN_MODE to_pin_mode(cDataBuffer& buffer)
    {
        uint8_t mode = 0;
        buffer >> mode;
        return static_cast<ouster::eIO_PIN_MODE>(mode);
    };

    ouster::azimuth_status to_azimuth_status(cDataBuffer& buffer)
    {
        int32_t status = 0;
        buffer >> status;
        return static_cast<ouster::azimuth_status>(status);
    };

    void to_LidarDataFormat_2(cDataBuffer& buffer, lidar_data_format_2_t& data)
    {
        buffer >> data.columns_per_frame;
        buffer >> data.columns_per_packet;

        uint32_t n = 0;
        buffer >> n;
        data.pixel_shift_by_row.resize(n);
        for (std::size_t i = 0; i < n; ++i)
            buffer >> data.pixel_shift_by_row[i];

        buffer >> data.pixels_per_column;
        buffer >> data.column_window_min;
        buffer >> data.column_window_max;
    }

#ifdef USE_OUSTER_DEFS
    void to_LidarData_2(cDataBuffer& buffer, cOusterLidarData& data)
    {
        uint16_t pixels_per_column = 0;
        uint16_t columns_per_frame = 0;

        buffer >> pixels_per_column;
        buffer >> columns_per_frame;

        data.resize(pixels_per_column, columns_per_frame);

        lidar_data_block_t pixel;

        for (uint16_t col = 0; col < columns_per_frame; ++col)
        {
            for (uint16_t chn = 0; chn < pixels_per_column; ++chn)
            {
                buffer >> pixel.range_mm;
                buffer >> pixel.signal;
                buffer >> pixel.reflectivity;
                buffer >> pixel.nir;
                data.channel(col, chn, pixel);
            }
        }
    }
#else
    void to_LidarData_2(cDataBuffer& buffer, lidar_data_frame_t& data)
    {
        buffer >> data.pixels_per_column;
        buffer >> data.columns_per_frame;

        data.channels.resize(data.columns_per_frame);

        for (uint16_t col = 0; col < data.columns_per_frame; ++col)
        {
            data.channels[col].pixels.resize(data.pixels_per_column);

            for (uint16_t p = 0; p < data.pixels_per_column; ++p)
            {
                auto& pixel = data.channels[col].pixels[p];
                buffer >> pixel.range_mm;
                buffer >> pixel.signal;
                buffer >> pixel.reflectivity;
                buffer >> pixel.ambient_noise;
            }
        }


    }
#endif
}

cOusterParser::cOusterParser()
:
    cBlockParser()
{}

cBlockID& cOusterParser::blockID()
{
    return mBlockID;
}


void cOusterParser::processData(BLOCK_MAJOR_VERSION_t major_version,
                                BLOCK_MINOR_VERSION_t minor_version,
                                BLOCK_DATA_ID_t data_id,
                                cDataBuffer& buffer)
{
    mBlockID.setVersion(major_version, minor_version);
    mBlockID.dataID(static_cast<ouster::DataID>(data_id));

    switch (static_cast<ouster::DataID>(data_id))
    {
    case DataID::CONFIGURATION_INFO:
        processConfigParam_2(buffer);
        break;
    case DataID::SENSOR_INFO:
        processSensorInfo_2(buffer);
        break;
    case DataID::TIMESTAMP:
        processTimestamp_2(buffer);
        break;
    case DataID::SYNC_PULSE_IN:
        processSyncPulseIn_2(buffer);
        break;
    case DataID::SYNC_PULSE_OUT:
        processSyncPulseOut_2(buffer);
        break;
    case DataID::MULTIPURPOSE_IO:
        processMultipurposeIO_2(buffer);
        break;
    case DataID::NMEA:
        processNmea_2(buffer);
        break;
    case DataID::TIME_INFO:
        processTimeInfo_2(buffer);
        break;
    case DataID::BEAM_INTRINSICS:
        processBeamIntrinsics_2(buffer);
        break;
    case DataID::IMU_INTRINSICS:
        processImuIntrinsics_2(buffer);
        break;
    case DataID::LIDAR_INTRINSICS:
        processLidarIntrinsics_2(buffer);
        break;
    case DataID::LIDAR_DATA_FORMAT:
        if (minor_version == 3)
            processLidarDataFormat_2_3(buffer);
        else
            processLidarDataFormat_2(buffer);
        break;
    case DataID::IMU_DATA:
        processImuData(buffer);
        break;
    case DataID::LIDAR_DATA:
        processLidarData(buffer);
        break;
    case DataID::LIDAR_DATA_FRAME_TIMESTAMP:
        processLidarDataFrameTimestamp(buffer);
        break;
    }
}

void cOusterParser::processConfigParam_2(cDataBuffer& buffer)
{
    config_param_2_t data;
    buffer >> data.udp_ip;
    buffer >> data.udp_dest;
    buffer >> data.lidar_port;
    buffer >> data.imu_port;

    data.timestamp_mode = to_timestamp_mode(buffer);
    data.sync_pulse_in_polarity = to_polarity(buffer);
    data.nmea_in_polarity = to_polarity(buffer);

    buffer >> data.nmea_ignore_valid_char;

    data.nmea_baud_rate = to_nmea_baud_rate(buffer);

    buffer >> data.nmea_leap_seconds;

    data.multipurpose_io_mode = to_pin_mode(buffer);
    data.sync_pulse_out_polarity = to_polarity(buffer);

    buffer >> data.sync_pulse_out_frequency_hz;
    buffer >> data.sync_pulse_out_angle;
    buffer >> data.sync_pulse_out_pulse_width;
    buffer >> data.auto_start_flag;

    data.operating_mode = to_operating_mode(buffer);
    data.lidar_mode = to_lidar_mode(buffer);

    buffer >> data.azimuth_window.min_deg;
    buffer >> data.azimuth_window.max_deg;
    buffer >> data.phase_lock_enable;
    buffer >> data.phase_lock_offset_deg;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processConfigParam_2.");

    onConfigParam_2(data);
}

void cOusterParser::processSensorInfo_2(cDataBuffer& buffer)
{
    sensor_info_2_t data;

    buffer >> data.product_line;
    buffer >> data.product_part_number;
    buffer >> data.product_serial_number;
    buffer >> data.base_part_number;
    buffer >> data.base_serial_number;
    buffer >> data.image_rev;

    data.build_revision = to_version(buffer);
    data.proto_revision = to_version(buffer);

    buffer >> data.build_date;

    data.status = to_sensor_status(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processSensorInfo_2.");

    onSensorInfo_2(data);
}

void cOusterParser::processTimestamp_2(cDataBuffer& buffer)
{
    timestamp_2_t data;

    buffer >> data.time;

    data.mode = to_timestamp_mode(buffer);

    buffer >> data.sync_pulse_in;
    buffer >> data.internal_osc;
    buffer >> data.ptp_1588;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processTimestamp_2.");

    onTimestamp_2(data);
}

void cOusterParser::processSyncPulseIn_2(cDataBuffer& buffer)
{
    sync_pulse_in_2_t data;

    buffer >> data.locked;
    buffer >> data.last_period_nsec;
    buffer >> data.count_unfiltered;
    buffer >> data.count;

    data.polarity = to_polarity(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processSyncPulseIn_2.");

    onSyncPulseIn_2(data);
}

void cOusterParser::processSyncPulseOut_2(cDataBuffer& buffer)
{
    sync_pulse_out_2_t data;

    buffer >> data.pulse_width_ms;
    buffer >> data.angle_deg;
    buffer >> data.frequency_hz;

    data.polarity = to_polarity(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processSyncPulseOut_2.");

    onSyncPulseOut_2(data);
}

void cOusterParser::processMultipurposeIO_2(cDataBuffer& buffer)
{
    multipurpose_io_2_t data;

    data.mode = to_pin_mode(buffer);

    buffer >> data.pulse_width_ms;
    buffer >> data.angle_deg;
    buffer >> data.frequency_hz;

    data.polarity = to_polarity(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processMultipurposeIO_2.");

    onMultipurposeIo_2(data);
}

void cOusterParser::processNmea_2(cDataBuffer& buffer)
{
    nmea_2_t data;

    buffer >> data.locked;
    data.baud_rate = to_nmea_baud_rate(buffer);
    buffer >> data.bit_count;
    buffer >> data.bit_count_unfiltered;
    buffer >> data.start_char_count;
    buffer >> data.char_count;
    buffer >> data.last_read_message;
    buffer >> data.date_decoded_count;
    buffer >> data.not_valid_count;
    buffer >> data.utc_decoded_count;
    buffer >> data.leap_seconds;
    buffer >> data.ignore_valid_char;
    data.polarity = to_polarity(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processNmea_2.");

    onNmea_2(data);
}

void cOusterParser::processTimeInfo_2(cDataBuffer& buffer)
{
    time_info_2_t data;

    /* Timestamp Info */
    buffer >> data.timestamp_info.time;
    data.timestamp_info.mode = to_timestamp_mode(buffer);
    buffer >> data.timestamp_info.sync_pulse_in;
    buffer >> data.timestamp_info.internal_osc;
    buffer >> data.timestamp_info.ptp_1588;

    /* Sync Pulse Info */
    buffer >> data.sync_pulse_info.locked;
    buffer >> data.sync_pulse_info.last_period_nsec;
    buffer >> data.sync_pulse_info.count_unfiltered;
    buffer >> data.sync_pulse_info.count;
    data.sync_pulse_info.polarity = to_polarity(buffer);

    /* Multipurpose IO Info */
    data.multipurpose_io_info.mode = to_pin_mode(buffer);
    buffer >> data.multipurpose_io_info.pulse_width_ms;
    buffer >> data.multipurpose_io_info.angle_deg;
    buffer >> data.multipurpose_io_info.frequency_hz;
    data.multipurpose_io_info.polarity = to_polarity(buffer);

    /* NMEA Info */
    buffer >> data.nmea_info.locked;
    data.nmea_info.baud_rate = to_nmea_baud_rate(buffer);
    buffer >> data.nmea_info.bit_count;
    buffer >> data.nmea_info.bit_count_unfiltered;
    buffer >> data.nmea_info.start_char_count;
    buffer >> data.nmea_info.char_count;
    buffer >> data.nmea_info.last_read_message;
    buffer >> data.nmea_info.date_decoded_count;
    buffer >> data.nmea_info.not_valid_count;
    buffer >> data.nmea_info.utc_decoded_count;
    buffer >> data.nmea_info.leap_seconds;
    buffer >> data.nmea_info.ignore_valid_char;
    data.nmea_info.polarity = to_polarity(buffer);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processTimeInfo_2.");

    onTimeInfo_2(data);
}

void cOusterParser::processBeamIntrinsics_2(cDataBuffer& buffer)
{
    beam_intrinsics_2_t data;

    buffer >> data.lidar_to_beam_origins_mm;

    uint32_t n = 0;
    buffer >> n;
    data.azimuth_angles_deg.resize(n);

    for (std::size_t i = 0; i < n; ++i)
        buffer >> data.azimuth_angles_deg[i];

    n = 0;
    buffer >> n;
    data.altitude_angles_deg.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        buffer >> data.altitude_angles_deg[i];

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processBeamIntrinsics_2.");

    onBeamIntrinsics_2(data);
}

void cOusterParser::processImuIntrinsics_2(cDataBuffer& buffer)
{
    imu_intrinsics_2_t data;

    uint32_t n = 0;
    buffer >> n;
    data.imu_to_sensor_transform.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        buffer >> data.imu_to_sensor_transform[i];

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processImuIntrinsics_2.");

    onImuIntrinsics_2(data);
}

void cOusterParser::processLidarIntrinsics_2(cDataBuffer& buffer)
{
    lidar_intrinsics_2_t data;

    uint32_t n = 0;
    buffer >> n;
    data.lidar_to_sensor_transform.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        buffer >> data.lidar_to_sensor_transform[i];

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarIntrinsics_2.");

    onLidarIntrinsics_2(data);
}

void cOusterParser::processLidarDataFormat_2(cDataBuffer& buffer)
{
    lidar_data_format_2_t data;

    to_LidarDataFormat_2(buffer, data);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarDataFormat_2.");

    onLidarDataFormat_2(data);
}

void cOusterParser::processImuData(cDataBuffer& buffer)
{
    imu_data_t data;

    buffer >> data.diagnostic_time_ns;
    buffer >> data.accelerometer_read_time_ns;
    buffer >> data.gyroscope_read_time_ns;

    buffer >> data.acceleration_Xaxis_g;
    buffer >> data.acceleration_Yaxis_g;
    buffer >> data.acceleration_Zaxis_g;

    buffer >> data.angular_velocity_Xaxis_deg_per_sec;
    buffer >> data.angular_velocity_Yaxis_deg_per_sec;
    buffer >> data.angular_velocity_Zaxis_deg_per_sec;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processImuData.");

    onImuData(data);
}

#ifdef USE_OUSTER_DEFS
void cOusterParser::processLidarDataFormat_2_3(cDataBuffer& buffer)
{
    lidar_data_format_2_t data;

    to_LidarDataFormat_2(buffer, data);

    buffer >> data.udp_profile_lidar;
    buffer >> data.udp_profile_imu;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarDataFormat_2_3.");

    onLidarDataFormat_2(data);
}

void cOusterParser::processLidarData(cDataBuffer& buffer)
{
    to_LidarData_2(buffer, mLidarData);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarData.");

    onLidarData(mLidarData);
}

void cOusterParser::processLidarDataFrameTimestamp(cDataBuffer& buffer)
{
    uint16_t frame_id = 0;
    uint64_t timestamp_ns = 0;

    buffer >> frame_id;
    buffer >> timestamp_ns;

    mLidarData.frame_id(frame_id);
    mLidarData.timestamp_ns(timestamp_ns);

    to_LidarData_2(buffer, mLidarData);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarDataFrameTimestamp.");

    onLidarData(mLidarData);
}

#else
void cOusterParser::processLidarDataFormat_2_3(cDataBuffer& buffer)
{
    lidar_data_format_2_3_t data;

    to_LidarDataFormat_2(buffer, data);

    buffer >> data.udp_profile_lidar;
    buffer >> data.udp_profile_imu;

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarDataFormat_2_3.");

    onLidarDataFormat_2(data);
}

void cOusterParser::processLidarData(cDataBuffer& buffer)
{
    static uint16_t frame_id = 0;

    lidar_data_frame_t data;

    data.frame_id = frame_id++;

    to_LidarData_2(buffer, data);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarData.");

    onLidarData(data);
}

void cOusterParser::processLidarDataFrameTimestamp(cDataBuffer& buffer)
{
    lidar_data_frame_t data;

    buffer >> data.frame_id;
    buffer >> data.timestamp_ns;

    to_LidarData_2(buffer, data);

    if (buffer.underrun())
        throw std::runtime_error("ERROR, Buffer under run in processLidarDataFrameTimestamp.");

    onLidarData(data);
}
#endif

