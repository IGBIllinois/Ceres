
#include "SensorFactory.hpp"

#include "Sensors/OusterFactory.hpp"
#include "Sensors/SsnxFactory.hpp"


sSensorWidgets create_sensor(const std::string& name)
{
    if (name.compare(ssnx_id) == 0)
    {
        return create_ssnx_sensor();
    }

    if (name.compare(ouster_id) == 0)
    {
        return create_ouster_sensor();
    }

    return sSensorWidgets();
}

void remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(ssnx_id) == 0)
    {
        remove_ssnx_sensor(widgets);
    }

    if (name.compare(ouster_id) == 0)
    {
        remove_ouster_sensor(widgets);
    }
}
