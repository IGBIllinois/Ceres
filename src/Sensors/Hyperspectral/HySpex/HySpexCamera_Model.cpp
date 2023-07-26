
#include "HySpexCamera_Model.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <optional>
#include <iostream>

cHySpexCameraModel::cHySpexCameraModel(const std::string& name, QObject* parent)
:
    cHyperspectralModel(name, parent)
{
    mConnected = false;
    mManufacturer = "HySpex";
}

const char* cHySpexCameraModel::descriptor() const
{
    return hyspex_id;
}

void cHySpexCameraModel::updateViews()
{
    emit initStatusChanged();
    emit commStatusChanged();
    emit acqStatusChanged();
    emit bgStatusChanged();
    emit coolingStatusChanged();
    emit shutterStatusChanged();

    emit imageSizeChanged(mSpatialSize, mSpectralSize);

    emit avgFramesChanged(mAverageFrames);
    emit framePeriodChanged(mFramePeriod_us);
    emit minFramePeriodChanged(mMinFramePeriod_us);
    emit integrationTimeChanged(mIntegrationTime_us);
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);

    emit lensInfoChanged();
}

bool cHySpexCameraModel::configure(const nlohmann::json& jsonCfg)
{
    if (!jsonCfg.contains("lens"))
        throw std::logic_error("Missing \"lens\" entry.");
    mLens = jsonCfg["lens"];

    return cHyperspectralModel::configure(jsonCfg);
}

hyspex::InitStatus cHySpexCameraModel::getInitStatus() const { return mInitStatus; }
hyspex::AcquisitionStatus cHySpexCameraModel::getAcquisitionStatus() const { return mAcquisitionStatus; }
hyspex::BackgroundStatus cHySpexCameraModel::getBackgroundStatus() const { return mBackgroundStatus; }
hyspex::CommunicationStatus cHySpexCameraModel::getCommunicationStatus() const { return mCommStatus; }
hyspex::CoolingStatus cHySpexCameraModel::getCoolingStatus() const { return mCoolingStatus; }
hyspex::ShutterStatus cHySpexCameraModel::getShutterStatus() const { return mShutterStatus; }

hyspex::WavelengthRangeId cHySpexCameraModel::getWavelengthRangeId() const { return mWavelengthRangeId; }

std::size_t cHySpexCameraModel::getSpatialSize() const { return mSpatialSize; }
std::size_t cHySpexCameraModel::getSpectralSize() const { return mSpectralSize; }
std::size_t cHySpexCameraModel::getMaxSpatialSize() const { return mMaxSpatialSize; }
std::size_t cHySpexCameraModel::getMaxSpectralSize() const { return mMaxSpectralSize; }
unsigned short cHySpexCameraModel::getMaxPixelValue() const { return mMaxPixelValue; }

double cHySpexCameraModel::getAmbientTemp_C() const { return mAmbientTemp_C; }
double cHySpexCameraModel::getSensorTemp_C() const { return mSensorTemp_C; }

/*
 * Retrieve the current acquistion information
 */
std::uint16_t cHySpexCameraModel::getAverageFrames() const { return mAverageFrames; }
std::uint32_t cHySpexCameraModel::getFramePeriod_us() const { return mFramePeriod_us; }
std::uint32_t cHySpexCameraModel::getMinFramePeriod_us() const { return mMinFramePeriod_us; }
std::uint32_t cHySpexCameraModel::getIntegrationTime_us() const { return mIntegrationTime_us; }
std::uint32_t cHySpexCameraModel::getMaxIntegrationTime_us() const { return mMaxIntegrationTime_us; }

/*
 * Retrieve the current lens information
 */
std::string cHySpexCameraModel::getLensName() const { return mLens; }
const std::vector<std::string>& cHySpexCameraModel::getLensNames() const { return mLenses; }
double cHySpexCameraModel::getWorkingDistance_cm() const { return mWorkingDistance_cm; }
double cHySpexCameraModel::getFieldOfView_deg() const { return mFieldOfView_deg;  }

std::uint32_t cHySpexCameraModel::getNumOfBackgrounds() const { return mNumBackgrounds; }



void cHySpexCameraModel::handleStatusCallback(void* p, int eventId, int value)
{
    using namespace hyspex;

    cHySpexCameraModel* self = static_cast<cHySpexCameraModel*>(p);

    switch (static_cast<EventType>(eventId))
    {
    case HYSPEX_EVENT_ACQUISITION_STATUS_CHANGED:    //!< Acquisition status changed, check Camera::getAcquisitionStatus().
        self->updateAcquisitionStatus(static_cast<AcquisitionStatus>(value));
        break;
    case HYSPEX_EVENT_COOLING_STATUS_CHANGED:        //!< Cooling status changed, check Camera::getCoolingStatus().
        self->updateCoolingStatus(static_cast<CoolingStatus>(value));
        break;
    case HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED:     //!< Background status changed, check Camera::getBackgroundStatus().
        self->updateBackgroundStatus(static_cast<BackgroundStatus>(value));
        break;
    case HYSPEX_EVENT_INIT_STATUS_CHANGED:           //!< Init status changed, check Camera::getInitStatus().
        self->updateInitStatus(static_cast<InitStatus>(value));
        break;
    case HYSPEX_EVENT_SHUTTER_STATUS_CHANGED:        //!< Shutter status changed, check Camera::getShutterStatus();
        self->updateShutterStatus(static_cast<ShutterStatus>(value));
        break;
    case HYSPEX_EVENT_COMMUNICATION_STATUS_CHANGED:  //!< Communication status changed. check Camera::getCommunicationStatus().
        self->updateCommStatus(static_cast<CommunicationStatus>(value));
        break;
    case HYSPEX_EVENT_MANUAL_SHUTTER_OPEN_REQUEST:   //!< Manual Shutter: open requested.
    case HYSPEX_EVENT_MANUAL_SHUTTER_CLOSE_REQUEST:  //!< Manual Shutter: close requested.
    case HYSPEX_EVENT_ERROR:                         //!< Future: unused.
    case HYSPEX_EVENT_WARNING:                       //!< Future: unused.
        break;
    }
}


void cHySpexCameraModel::updateInitStatus(hyspex::InitStatus status)
{
    mInitStatus = status;
    emit initStatusChanged();
}

void cHySpexCameraModel::updateCommStatus(hyspex::CommunicationStatus status)
{
    mCommStatus = status;
    emit commStatusChanged();
}

void cHySpexCameraModel::updateCoolingStatus(hyspex::CoolingStatus status)
{
    mCoolingStatus = status;

    if (getStatus() == sensor::eStatus::BUSY)
    {
        if ((mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_OK) ||
            (mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_DEGRADED))
            setStatus(sensor::eStatus::RUNNING);
    }
    emit coolingStatusChanged();
}

void cHySpexCameraModel::updateBackgroundStatus(hyspex::BackgroundStatus status)
{
    mBackgroundStatus = status;
    emit bgStatusChanged();
}

void cHySpexCameraModel::updateAcquisitionStatus(hyspex::AcquisitionStatus status)
{
    mAcquisitionStatus = status;
    emit acqStatusChanged();
}

void cHySpexCameraModel::updateShutterStatus(hyspex::ShutterStatus status)
{
    mShutterStatus = status;
    emit shutterStatusChanged();
}

