
#pragma once

#include "Lidar/Ouster/OusterModel.hpp"

#include <cbdf/OusterParser.hpp>

#include <ouster/ouster_defs.h>

#include <QObject>

class cOusterModel_file : public cOusterModel, public cOusterParser
{
    Q_OBJECT

public:
    cOusterModel_file(QObject* parent = nullptr);
    virtual ~cOusterModel_file() = default;

protected:
	void onConfigParam(ouster::config_param_2_t config_param) override;
	void onSensorInfo(ouster::sensor_info_2_t sensor_info) override;
	void onTimestamp(ouster::timestamp_2_t timestamp) override;
	void onSyncPulseIn(ouster::sync_pulse_in_2_t pulse_info) override;
	void onSyncPulseOut(ouster::sync_pulse_out_2_t pulse_info) override;
	void onMultipurposeIo(ouster::multipurpose_io_2_t io) override;
	void onNmea(ouster::nmea_2_t nmea) override;
	void onTimeInfo(ouster::time_info_2_t time_info) override;
	void onBeamIntrinsics(ouster::beam_intrinsics_2_t intrinsics) override;
	void onImuIntrinsics(ouster::imu_intrinsics_2_t intrinsics) override;
	void onLidarIntrinsics(ouster::lidar_intrinsics_2_t intrinsics) override;
	void onLidarDataFormat(ouster::lidar_data_format_2_t format) override;
	void onImuData(ouster::imu_data_t data) override;
	void onLidarData(cOusterLidarData data) override;

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

