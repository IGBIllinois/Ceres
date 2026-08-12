
#pragma once

#include "../SensorModel.hpp"

#include "Timers.hpp"

#include <optional>


class cRgbCameraModel : public cSensorModel
{
public:
    enum eMode { SINGLE = 0, TIME_LAPSE = 1, CONTINUOUS = 2 };

public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "rgb"; };

    /*
     * General accessors and control of basic camera functions
     */

    eMode mode() const;
    virtual void setMode(eMode mode) = 0;

    // Get/Set the frame rate when the mode is set to continuous
    double frameRate_Hz() const;
    virtual void setFrameRate_Hz(double frame_rate_hz) = 0;

    std::optional<double> minFrameRate_fps() const;
    std::optional<double> maxFrameRate_fps() const;

    // Get/Set the lapse interval (the time between images) when the mode is set to time-lapse
    uint32_t lapseInterval_ms() const;
    virtual void setLapseInterval_ms(uint32_t interval_ms) = 0;

protected:
    cRgbCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cRgbCameraModel() = default;

protected:
    eMode mMode = eMode::CONTINUOUS;

    double mFrameRate_fps = 0;
    std::optional<double> mMinFrameRate_fps;
    std::optional<double> mMaxFrameRate_fps;

    uint32_t mLapseInterval_ms = 0;
    cOneShotTimer mTimeLapseTimer;
};

