
#pragma once

#include <QString>

#include <string>


namespace sensor
{
    enum class eStatus 
    { UNKNOWN, CONFIGURED, INITIALIZED, CONNECTING, CONNECTED, WARM_UP, 
        RUNNING, STOPPED, FAILED, REINITIALIZING, BUSY, WAITING, PENDING };

    std::string to_string(eStatus status);
    eStatus to_sensor_status(const std::string& str);
    eStatus to_sensor_status(const QString& str);
}

