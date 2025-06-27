
#include "SensorDefs.hpp"


namespace sensor
{
    std::string to_string(eStatus status)
    {
        switch (status)
        {
        case eStatus::UNKNOWN:
            return "UNKNOWN";
        case eStatus::CONFIGURED:
            return "CONFIGURED";
        case eStatus::INITIALIZED:
            return "INITIALIZED";
        case eStatus::CONNECTING:
            return "CONNECTING";
        case eStatus::CONNECTED:
            return "CONNECTED";
        case eStatus::WARM_UP:
            return "WARM UP";
        case eStatus::RUNNING:
            return "RUNNING";
        case eStatus::STOPPED:
            return "STOPPED";
        case eStatus::FAILED:
            return "FAILED";
        case eStatus::REINITIALIZING:
            return "REINITIALIZING";
        case eStatus::BUSY:
            return "BUSY";
        case eStatus::WAITING:
            return "WAITING";
        case eStatus::PENDING:
            return "PENDING";
        }

        return "UNKNOWN";
    }

    eStatus to_sensor_status(const std::string& str)
    {
        if (str == "CONFIGURED" || str == "configured")
            return eStatus::CONFIGURED;
        if (str == "INITIALIZED" || str == "initialized")
            return eStatus::INITIALIZED;
        if (str == "CONNECTING" || str == "connecting")
            return eStatus::CONNECTING;
        if (str == "CONNECTED" || str == "connected")
            return eStatus::CONNECTED;
        if (str == "WARM UP" || str == "warm up")
            return eStatus::WARM_UP;
        if (str == "RUNNING" || str == "running")
            return eStatus::RUNNING;
        if (str == "STOPPED" || str == "stopped")
            return eStatus::STOPPED;
        if (str == "FAILED" || str == "failed")
            return eStatus::FAILED;
        if (str == "REINITIALIZING" || str == "reinitializing")
            return eStatus::REINITIALIZING;
        if (str == "BUSY" || str == "busy")
            return eStatus::BUSY;
        if (str == "WAITING" || str == "waiting")
            return eStatus::WAITING;
        if (str == "PENDING" || str == "pending")
            return eStatus::PENDING;

        return eStatus::UNKNOWN;
    }

    eStatus to_sensor_status(const QString& str)
    {
        return to_sensor_status(str.toStdString());
    }

}

