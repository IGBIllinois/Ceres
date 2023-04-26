/**
 * @file
 */
#pragma once

#include <cbdf/BlockDataFile.hpp>
#include <cbdf/OusterParser.hpp>
#include <cbdf/AxisCommunicationsParser.hpp>

#include <QObject>
#include <QRunnable>

class cDataVerifier : public QObject, public QRunnable, 
                        public cOusterParser, public cAxisCommunicationsParser
{
    Q_OBJECT

public:
    cDataVerifier(int id, const QString& dataDir, QObject* parent = nullptr);
    ~cDataVerifier();

    bool open(const std::string& file_name);

    void run() override;

signals:
    void statusMessage(QString msg);
    void fileResults(int id, bool valid, QString msg);

protected:
	void moveFileToFailed();

private:
	void onConfigParam(ouster::config_param_2_t data) override;
	void onSensorInfo(ouster::sensor_info_2_t data) override;
	void onTimestamp(ouster::timestamp_2_t data) override;
	void onSyncPulseIn(ouster::sync_pulse_in_2_t data) override;
	void onSyncPulseOut(ouster::sync_pulse_out_2_t data) override;
	void onMultipurposeIo(ouster::multipurpose_io_2_t data) override;
	void onNmea(ouster::nmea_2_t data) override;
	void onTimeInfo(ouster::time_info_2_t data) override;
	void onBeamIntrinsics(ouster::beam_intrinsics_2_t data) override;
	void onImuIntrinsics(ouster::imu_intrinsics_2_t data) override;
	void onLidarIntrinsics(ouster::lidar_intrinsics_2_t data) override;
	void onLidarDataFormat(ouster::lidar_data_format_2_t data) override;
	void onImuData(ouster::imu_data_t data) override;
	void onLidarData(cOusterLidarData data) override;

	void onActiveCameraId(int id) override;
	void onFramesPerSecond(int frames_per_sec) override;
	void onBitmap(const cBitmapBuffer& buffer) override;
	void onJPEG(const cJpegBuffer& buffer) override;
	void onMpegFrame(const cMpegFrameBuffer& buffer) override;
	void onImageSize(int width, int height) override;

private:
	const int mId;
    cBlockDataFileReader mFileReader;

	QString     mCurrentDataDirectory;
	QString     mCurrentFileName;
};

