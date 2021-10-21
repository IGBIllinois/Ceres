
#include "SensorFactory.hpp"

#include "Sensors/OusterFactory.hpp"
#include "Sensors/SsnxFactory.hpp"


std::pair<cSensorModel*, QDockWidget*> create_sensor(const std::string& name, QWidget* parent)
{
    if (name.compare(ssnx_id) == 0)
    {
        return create_ssnx_sensor(parent);
    }

    if (name.compare(ouster_id) == 0)
    {
        return create_ouster_sensor(parent);
    }

    return std::make_pair(nullptr, nullptr);
}