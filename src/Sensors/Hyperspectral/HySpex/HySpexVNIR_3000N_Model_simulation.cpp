
#include "HySpexVNIR_3000N_Model_simulation.hpp"
#include "Constants.hpp"

#include <HySpexConnect/hyspex_utils.hpp>

#include <QDebug>

#include <optional>

using namespace hyspex;

cHySpexVNIR_3000N_Model_simulation::cHySpexVNIR_3000N_Model_simulation(QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent)
{
    mTemperatureUpdateTimer.interval_sec(5);
    mShutterTimer.interval_sec(1);
    mBackgroundTimer.interval_sec(4);
    mSimDataUpdateTimer.interval_ms(18);
}

cHySpexVNIR_3000N_Model_simulation::~cHySpexVNIR_3000N_Model_simulation()
{
}


void cHySpexVNIR_3000N_Model_simulation::updateViews()
{
    cHySpexVNIR_3000N_Model::updateViews();

    emit lensInfoChanged();
}

bool cHySpexVNIR_3000N_Model_simulation::configure(const nlohmann::json& jsonCfg)
{
    mSerialNumber = "6500";

    mLenses.clear();
    mLenses.emplace_back("1 m");
    mLenses.emplace_back("3 m");
    mLenses.emplace_back("Plane");

    try
    {
        auto section = jsonCfg["VNIR-3000N"];

        cHySpexVNIR_3000N_Model::configure(section);
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex, VNIR-3000N\" camera configuration section: ";
        msg.append(e.what());
        qCritical() << msg;

        emit logMessage(logERROR, q_name(), msg);

        setStatus(sensor::eStatus::FAILED);

        return false;
    }

    mLens = "3 m";
    mWorkingDistance_cm = 300;
    mFieldOfView_deg = 18.745;
    emit lensInfoChanged();

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

bool cHySpexVNIR_3000N_Model_simulation::initialize()
{
    emit statusMessage("Retrieving HySpex VNIR-3000N camera configuration...");

    mInitStatus = hyspex::InitStatus::HYSPEX_INIT_PENDING_SENSOR;
    emit initStatusChanged();

    mInitStatus = hyspex::InitStatus::HYSPEX_INIT_OK;
    emit initStatusChanged();

    mWavelengthRangeId = hyspex::WavelengthRangeId::HYSPEX_WRID_VNIR;

    mSpectralSize = 700;
    mSpatialSize = 3408;

    mMaxSpatialSize = 700;
    mMaxSpectralSize = 3408;

    mMaxPixelValue = 4095;
    mSaturationValue = mMaxPixelValue - 2;

    mCommStatus = hyspex::CommunicationStatus::HYSPEX_COMM_OK;
    emit commStatusChanged();

	mCoolingStatus = hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_OK;
    emit coolingStatusChanged();

    mShutterStatus = hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN;
    emit shutterStatusChanged();

	mAverageFrames = 1;
    emit avgFramesChanged(mAverageFrames);

	mFramePeriod_us = 18182;
    mMinFramePeriod_us = 18182;
    emit minFramePeriodChanged(mMinFramePeriod_us);
    emit framePeriodChanged(mFramePeriod_us);

    mIntegrationTime_us = 10804;
    mMaxIntegrationTime_us = 12169;
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);
    emit integrationTimeChanged(mIntegrationTime_us);

    mAmbientTemp_C = 20.0;
    emit ambientTempChanged(mAmbientTemp_C);
	
    mSensorTemp_C = 36.0;
    emit sensorTempChanged(mSensorTemp_C);

    mNumBackgrounds = 200;
	mBackgroundStatus = hyspex::BackgroundStatus::HYSPEX_BG_INVALID;
    emit bgStatusChanged();

	mAcquisitionStatus = hyspex::AcquisitionStatus::HYSPEX_ACQ_PENDING;
    emit acqStatusChanged();

    mSpectralCalibrationPerBand.resize(700);
    for (int i = 0; i < 700; ++i)
        mSpectralCalibrationPerBand[i] = 0.846498 * i + 404.895;

    return cHySpexVNIR_3000N_Model::initialize();
}


bool cHySpexVNIR_3000N_Model_simulation::startCommunications()
{
    mTemperatureUpdateTimer.reset();
    mSimDataUpdateTimer.reset();

    mAcquisitionStatus = hyspex::AcquisitionStatus::HYSPEX_ACQ_RUNNING;
    emit acqStatusChanged();

    mConnected = (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_PENDING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_RUNNING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED);

    if (mConnected)
    {
        if ((mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_OK) || 
            (mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_DEGRADED))
            setStatus(sensor::eStatus::RUNNING);
        else
            setStatus(sensor::eStatus::BUSY);
    }
    else
        setStatus(sensor::eStatus::FAILED);

    return mConnected;
}

void cHySpexVNIR_3000N_Model_simulation::stopCommunications()
{
    mTemperatureUpdateTimer.stop();
    mSimDataUpdateTimer.stop();

    mAcquisitionStatus = hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED;
    emit acqStatusChanged();

    mConnected = false;

    setStatus(sensor::eStatus::STOPPED);
}

void cHySpexVNIR_3000N_Model_simulation::update()
{
    if (mTemperatureUpdateTimer.elapsed())
    {
        mAmbientTemp_C = 20.2;
        emit ambientTempChanged(mAmbientTemp_C);

        mSensorTemp_C = 36.2;
        emit sensorTempChanged(mSensorTemp_C);
    }

    if (mBackgroundState != eBgStates::NONE)
    {
        switch (mBackgroundState)
        {
        case eBgStates::SH_CLOSE:
        {
            if (mShutterTimer.elapsed())
            {
                mBackgroundStatus = hyspex::BackgroundStatus::HYSPEX_BG_PENDING;
                mBackgroundTimer.reset();
                mBackgroundState = eBgStates::COMPLETE;
            }
            break;
        }
        case eBgStates::COMPLETE:
        {
            if (mBackgroundTimer.elapsed())
            {
                mShutterTimer.stop();
                mBackgroundState = eBgStates::SH_OPEN;
                break;
            }

            break;
        }
        case eBgStates::SH_OPEN:
        {
            if (mShutterTimer.elapsed())
            {
                mBackgroundState = eBgStates::NONE;
                emit backgroundComplete();
            }
            break;
        }
        case eBgStates::ABORT:
        {
            mBackgroundState = eBgStates::SH_OPEN;
            break;
        }
        }

        return;
    }
}

void cHySpexVNIR_3000N_Model_simulation::writeDataHeader()
{
}

void cHySpexVNIR_3000N_Model_simulation::setAcquisitionParameters(std::uint16_t avg_frames,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    auto prevAverageFrames = mAverageFrames;
    mAverageFrames = avg_frames;

    auto prevMaxIntegrationTime_us = mMaxIntegrationTime_us;
    mMaxIntegrationTime_us = integration_time_us + 10;
    if (integration_time_us > mMaxIntegrationTime_us)
        integration_time_us = mMaxIntegrationTime_us;

    auto prevIntegrationTime_us = mIntegrationTime_us;
    auto prevMinFramePeriod_us = mMinFramePeriod_us;
    mIntegrationTime_us = integration_time_us;
    mMinFramePeriod_us = integration_time_us + 490;

    if (frame_period_us < mMinFramePeriod_us)
        frame_period_us = mMinFramePeriod_us;

    auto prevFramePeriod_us = mFramePeriod_us;
    mSimDataUpdateTimer.interval_ms(frame_period_us / 1000);
    mFramePeriod_us = frame_period_us / 1000;

    if (prevAverageFrames != mAverageFrames)
        emit avgFramesChanged(mAverageFrames);

    if (prevMaxIntegrationTime_us != mMaxIntegrationTime_us)
        emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);

    if (prevMinFramePeriod_us != mMinFramePeriod_us)
        emit minFramePeriodChanged(mMinFramePeriod_us);

    if (prevFramePeriod_us != mFramePeriod_us)
        emit framePeriodChanged(mFramePeriod_us);

    if (prevIntegrationTime_us != mIntegrationTime_us)
        emit integrationTimeChanged(mIntegrationTime_us);
}

void cHySpexVNIR_3000N_Model_simulation::setNumOfBackgrounds(int num_backgrounds)
{
    if (num_backgrounds < 0) num_backgrounds = 0;
    if (num_backgrounds > 1000) num_backgrounds = 1000;
    mNumBackgrounds = num_backgrounds;
    mBackgroundTimer.interval_sec(static_cast<uint32_t>(0.02 * mNumBackgrounds));

}

void cHySpexVNIR_3000N_Model_simulation::calcBackground()
{
    mShutterTimer.reset();
    mBackgroundState = eBgStates::SH_CLOSE;
}

void cHySpexVNIR_3000N_Model_simulation::stopBackground()
{
    mBackgroundState = eBgStates::ABORT;
}

void cHySpexVNIR_3000N_Model_simulation::open_shutter()
{
    mShutterStatus = hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN;
    emit shutterStatusChanged();
}

void cHySpexVNIR_3000N_Model_simulation::close_shutter()
{
    mShutterStatus = hyspex::ShutterStatus::HYSPEX_SHUTTER_CLOSED;
    emit shutterStatusChanged();
}




