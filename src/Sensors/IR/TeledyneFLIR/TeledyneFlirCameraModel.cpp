
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirIDs.hpp"

#include "StringUtils.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>
#include <QByteArray>
#include <QUrlQuery>
#include <QImage>

#include <nlohmann/json.hpp>

namespace
{
    QByteArray to_QByteArray(const nlohmann::json& jsonDoc)
    {
        std::string s = jsonDoc.dump();
        return QByteArray(s.c_str(), s.size());
    }
}


cTeledyneFlirCameraModel::cTeledyneFlirCameraModel(const std::string& name, QObject* parent)
:
    cIrCameraModel(name, parent), mColorizedImage(640, 480, QImage::Format_RGB888)
{
    mManufacturer = "Teledyne";
}

cTeledyneFlirCameraModel::~cTeledyneFlirCameraModel()
{
//	stopCommunications();
}

const char* cTeledyneFlirCameraModel::descriptor() const
{
    return teledyne_flir_id;
}

uint16_t cTeledyneFlirCameraModel::data_class_id() const
{
    return mSerializer.classID();
}

bool cTeledyneFlirCameraModel::configure(const nlohmann::json& jsonCfg)
{
    if (!jsonCfg.contains("mode"))
        throw std::logic_error("Missing \"mode\" entry!  Values can be \"photo\", \"time lapse\", or \"video\".");
    auto mode = jsonCfg["mode"];

    double frame_rate_fps = -1;

    if (jsonCfg.contains("frame rate (hz)"))
        frame_rate_fps = jsonCfg["frame rate (hz)"].get<double>();

    int32_t lapse_interval_ms = -1;

    if (jsonCfg.contains("time-lapse interval (s)"))
        lapse_interval_ms = static_cast<int32_t>(jsonCfg["time-lapse interval (s)"].get<float>() * 1000.0);
    else if (jsonCfg.contains("time-lapse interval (ms)"))
        lapse_interval_ms = jsonCfg["time-lapse interval (ms)"].get<int32_t>();
    else if (jsonCfg.contains("lapse interval (s)"))
        lapse_interval_ms = static_cast<int32_t>(jsonCfg["lapse interval (s)"].get<float>() * 1000.0);
    else if (jsonCfg.contains("lapse interval (ms)"))
        lapse_interval_ms = jsonCfg["lapse interval (ms)"].get<int32_t>();
    else if (jsonCfg.contains("interval (s)"))
        lapse_interval_ms = static_cast<int32_t>(jsonCfg["interval (s)"].get<float>() * 1000.0);
    else if (jsonCfg.contains("interval (ms)"))
        lapse_interval_ms = jsonCfg["interval (ms)"].get<int32_t>();

    if (nStringUtils::iequal(mode, "photo"))
    {
        setMode(eMode::SINGLE);
    }
    else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
    {
        setMode(eMode::TIME_LAPSE);

        if (lapse_interval_ms < 0)
        {
            throw std::logic_error("Missing \"time-lapse interval (ms)\" entry!");
        }
    }
    else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
    {
        setMode(eMode::CONTINUOUS);

        if (frame_rate_fps < 0)
        {
            throw std::logic_error("Missing \"frame rate (hz)\" entry!");
        }

    }
    else
        throw std::logic_error("Unknown \"mode\" entry!  Values can be \"photo\", \"time lapse\", or \"video\".");

    if (lapse_interval_ms > 0)
        setLapseInterval_ms(lapse_interval_ms);

    if (frame_rate_fps > 0)
        setFrameRate_Hz(frame_rate_fps);

    return cIrCameraModel::configure(jsonCfg);
}


cTeledyneFlirCameraModel::eMode cTeledyneFlirCameraModel::mode() const { return mMode; }

void cTeledyneFlirCameraModel::setMode(eMode mode)
{
    bool changing = mode != mMode;

    mMode = mode;

    if (changing)
        emit modeChanged(static_cast<int>(mMode));
}

double  cTeledyneFlirCameraModel::frameRate_Hz() const
{
    return mFrameRate_fps;
}

void cTeledyneFlirCameraModel::setFrameRate_Hz(double frame_rate_hz)
{
    if (mMinFrameRate_fps.has_value())
    {
        if (frame_rate_hz < mMinFrameRate_fps.value())
            frame_rate_hz = mMinFrameRate_fps.value();
    }
    else if (frame_rate_hz < 0)
        frame_rate_hz = 0;

    if (mMaxFrameRate_fps.has_value())
    {
        if (frame_rate_hz > mMaxFrameRate_fps.value())
            frame_rate_hz = mMaxFrameRate_fps.value();
    }

    bool changing = frame_rate_hz != mFrameRate_fps;

    if (updateFrameRate(frame_rate_hz))
    {
        mFrameRate_fps = frame_rate_hz;

        if (changing)
            emit frameRateChanged(mFrameRate_fps);
    }
}

uint32_t cTeledyneFlirCameraModel::lapseInterval_ms() const { return mLapseInterval_ms; }

void cTeledyneFlirCameraModel::setLapseInterval_ms(uint32_t interval_ms)
{
    if (interval_ms < 100)
        interval_ms = 100;

    bool changing = interval_ms != mLapseInterval_ms;

    if (updateLapseInterval(interval_ms))
    {
        mLapseInterval_ms = interval_ms;

        if (changing)
            emit lapseIntervalChanged(mLapseInterval_ms);
    }
}

void cTeledyneFlirCameraModel::setImageSize(int width, int height)
{
    bool changing = (width != mImageWidth) || (height != mImageHeight);

    mImageWidth = width;
    mImageHeight = height;

    if (changing)
        emit imageSizeChanged(mImageWidth, mImageHeight);
}


void cTeledyneFlirCameraModel::queryMode()
{
    emit modeChanged(static_cast<int>(mMode));
}

void cTeledyneFlirCameraModel::queryFrameRate()
{
    emit frameRateChanged(mFrameRate_fps);
}

void cTeledyneFlirCameraModel::queryLapseInterval()
{
    emit lapseIntervalChanged(mLapseInterval_ms);
}

void cTeledyneFlirCameraModel::queryImageSize()
{
    emit imageSizeChanged(mImageWidth, mImageHeight);
}


void cTeledyneFlirCameraModel::requestMode(int mode)
{
    if ((mode >= eMode::SINGLE) && (mode <= eMode::CONTINUOUS))
        setMode(static_cast<eMode>(mode));
}

void cTeledyneFlirCameraModel::requestFrameRate_Hz(double frame_rate_hz)
{
    setFrameRate_Hz(frame_rate_hz);
}

void cTeledyneFlirCameraModel::requestLapseInterval_ms(uint32_t interval_ms)
{
    setLapseInterval_ms(interval_ms);
}

void cTeledyneFlirCameraModel::requestImageSize(int width, int height)
{
    setImageSize(width, height);
}

void cTeledyneFlirCameraModel::requestImage()
{
    mImageRequested = true;
}

void cTeledyneFlirCameraModel::requestImages(bool auto_emit)
{
    mAutoEmitImages = auto_emit;
}

void cTeledyneFlirCameraModel::takePhoto(bool update_view)
{
    if (mMode == eMode::SINGLE)
    {
        mPhotoRequested = true;
        mImageRequested = update_view;
        mAutoEmitImages = false;
        mSavePhoto      = false;
    }
}

void cTeledyneFlirCameraModel::takePhoto(bool update_view, bool auto_save)
{
    if (mMode == eMode::SINGLE)
    {
        mPhotoRequested = true;
        mImageRequested = update_view;
        mAutoEmitImages = false;
        mSavePhoto      = auto_save;
    }
}

void cTeledyneFlirCameraModel::onSaveState()
{}

void cTeledyneFlirCameraModel::onRestoreState()
{}

const QImage& cTeledyneFlirCameraModel::getCurrentImage() const
{
    return mColorizedImage;
}

