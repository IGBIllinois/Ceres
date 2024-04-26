
#pragma once

#include "../../SensorFactory.hpp"


static const char* hyspex_id = "hyspex";
static const char* vnir_3000N_id = "VNIR-3000N";
static const char* swir_384_id = "SWIR-384";

namespace hyspex
{
	sSensorWidgets create_sensor(const nlohmann::json& sensorInfo, bool no_visualization);

	bool remove_sensor(sSensorWidgets widgets);

	cSensorPropertyPage* create_sensor_property_page(const std::string& model, uint32_t version,
		const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);
}