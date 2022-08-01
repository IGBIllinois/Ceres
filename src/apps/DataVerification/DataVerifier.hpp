/**
 * @file
 */
#pragma once

#include "BlockDataFile.hpp"
#include "OusterParser.hpp"
#include "AxisCommunicationsParser.hpp"

#include <QObject>
#include <QRunnable>

class cDataVerifier : public QObject, public QRunnable, 
                        public cOusterParser, public cAxisCommunicationsParser
{
    Q_OBJECT

public:
    cDataVerifier(const QString& dataDir, QObject* parent = nullptr);
    ~cDataVerifier();

    bool open(const std::string& file_name);

    void run() override;

signals:
    void statusMessage(QString msg);
    void fileResults(bool valid, QString msg);

protected:
	void moveFileToFailed();

private:
	void onConfigParam_2(const ouster::config_param_2_t& data) override;
	void onSensorInfo_2(const ouster::sensor_info_2_t& data) override;
	void onTimestamp_2(const ouster::timestamp_2_t& data) override;
	void onSyncPulseIn_2(const ouster::sync_pulse_in_2_t& data) override;
	void onSyncPulseOut_2(const ouster::sync_pulse_out_2_t& data) override;
	void onMultipurposeIo_2(const ouster::multipurpose_io_2_t& data) override;
	void onNmea_2(const ouster::nmea_2_t& data) override;
	void onTimeInfo_2(const ouster::time_info_2_t& data) override;
	void onBeamIntrinsics_2(const ouster::beam_intrinsics_2_t& data) override;
	void onImuIntrinsics_2(const ouster::imu_intrinsics_2_t& data) override;
	void onLidarIntrinsics_2(const ouster::lidar_intrinsics_2_t& data) override;
	void onLidarDataFormat_2(const ouster::lidar_data_format_2_t& data) override;
	void onImuData(const ouster::imu_data_t& data) override;
	void onLidarDataFormat_2(const ouster::lidar_data_format_2_3_t& data) override;
	void onLidarData(const ouster::lidar_data_frame_t& data) override;

	void onActiveCameraId(int id) override;
	void onFramesPerSecond(int frames_per_sec) override;
	void onBitmap(const QBitmap& in) override;
	void onJPEG(const QImage& image) override;
	void onMpegFrame(const QImage& image) override;
	void onImageSize(int width, int height) override;

private:
    cBlockDataFileReader mFileReader;

	QString     mCurrentDataDirectory;
	QString     mCurrentFileName;
};

