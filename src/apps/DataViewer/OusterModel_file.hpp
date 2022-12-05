
#pragma once

#include "OusterParser.hpp"
#include "Lidar/Ouster/OusterModel.hpp"

#include <ouster/ouster_defs.h>

#include <QObject>

class cOusterModel_file : public cOusterModel, public cOusterParser
{
    Q_OBJECT

public:
    cOusterModel_file(QObject* parent = nullptr);
    virtual ~cOusterModel_file() = default;

protected:
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
	void onLidarData(const cOusterLidarData& data) override;

	void onNewData(const ouster::imu_data_t& new_data) override;
	void onNewData(uint16_t frameID, const cOusterLidarData& data) override;


	// Override of the virtual public slots in cOusterModel
	void changeAzimuthWindow(double min_deg, double max_deg) override {};
	void changeLidarMode(QString mode_str) override {};

private:
	uint16_t data_class_id() const override { return 0; }
	bool configure(const nlohmann::json&) override { return true; }
	void enableDataRecording(cBlockDataFileWriter&) override {}
	void disableDataRecording() override {}
	void writeDataHeader() override {}
	bool startCommunications() override { return true; }
	void stopCommunications() override {}
	void update() override {}
};

