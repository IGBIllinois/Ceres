
#include "LucidVisionLabsRgbModel.hpp"
#include "LucidVisionLabsIDs.hpp"

#include "StringUtils.hpp"

#include <nlohmann/json.hpp>


cLucidVisionLabsRgbModel::cLucidVisionLabsRgbModel(const std::string& name, QObject* parent)
:
    cRgbCameraModel(name, parent)
{
    mManufacturer = "Lucid Vision Labs";
}

cLucidVisionLabsRgbModel::~cLucidVisionLabsRgbModel()
{
	stopCommunications();
}

const char* cLucidVisionLabsRgbModel::descriptor() const
{
    return lucid_vision_labs_id;
}

uint16_t cLucidVisionLabsRgbModel::data_class_id() const
{
    return mSerializer.classID();
}

bool cLucidVisionLabsRgbModel::configure(const nlohmann::json& jsonCfg)
{
    if (!jsonCfg.contains("mode"))
        throw std::logic_error("Missing \"mode\" entry!  Values can be \"photo\", \"time lapse\", or \"video\".");
    auto mode = jsonCfg["mode"];

    double frame_rate_fps = -1;

    if (jsonCfg.contains("frame rate (hz)"))
        frame_rate_fps = jsonCfg["frame rate (hz)"].get<double>();

    int32_t lapse_interval_ms = -1;

    if (jsonCfg.contains("lapse interval (s)"))
        lapse_interval_ms = static_cast<int32_t>(jsonCfg["lapse interval (s)"].get<float>() * 1000.0);
    else if (jsonCfg.contains("lapse interval (ms)"))
        lapse_interval_ms = jsonCfg["lapse interval (ms)"].get<int32_t>();

    else if (jsonCfg.contains("interval (s)"))
        lapse_interval_ms = static_cast<int32_t>(jsonCfg["interval (s)"].get<float>() * 1000.0);
    else if (jsonCfg.contains("interval (ms)"))
        lapse_interval_ms = jsonCfg["interval (ms)"].get<int32_t>();

    if (nStringUtils::iequal(mode, "photo"))
    {
        mMode = eMode::SINGLE;
    }
    else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
    {
        mMode = eMode::TIME_LAPSE;

        if (lapse_interval_ms < 0)
        {
            throw std::logic_error("Missing \"interval (ms)\" entry!");
        }
    }
    else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
    {
        mMode = eMode::CONTINUOUS;
    }
    else
        throw std::logic_error("Unknown \"mode\" entry!  Values can be \"photo\", \"time lapse\", or \"video\".");

    if (lapse_interval_ms > 0)
        setLapseInterval_ms(lapse_interval_ms);

    if (frame_rate_fps > 0)
        setFrameRate_Hz(frame_rate_fps);

    setMode(mMode);

    return true;
}

void cLucidVisionLabsRgbModel::updateViews()
{
    emit sensorStatusChanging(q_name(), q_instance(), getStatus());

    emit modeChanged(static_cast<int>(mMode));
    emit lapseIntervalChanged(mLapseInterval_ms);
    emit frameRateChanged(mFrameRate_fps);
    emit imageSizeChanged(mImageWidth, mImageHeight);
}

bool cLucidVisionLabsRgbModel::startCommunications()
{
    return true;
}

void cLucidVisionLabsRgbModel::stopCommunications()
{
}

void cLucidVisionLabsRgbModel::requestImage()
{
}


