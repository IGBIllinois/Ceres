
#include "HySpexCamera_Model.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <optional>
#include <iostream>
#include <array>
#include <valarray>


namespace
{
    using namespace HySpexConnect;


    template<typename T>
    std::valarray<double> convolution1D(view<T>& f)
    {
        const std::array<double, 3> focus_convolution = { -1, 2, -1 };
        const int nf = f.size();
        constexpr int ng = focus_convolution.size();
        int const n = nf + ng - 1;
        
        // Note: we shrink n by four bacause we want to get rid of edge effects
        std::valarray<double> out;
        out.resize(n-4, 0.0);

        // Note: we start at 2 and goto n-2 to avoid edge effects
        for (auto i(2); i < n-2; ++i)
        {
            const int jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
            const int jmx = (i < nf - 1) ? i : nf - 1;
            for (auto j(jmn); j <= jmx; ++j) 
            {
                out[i-2] += (f[j] * focus_convolution[i - j]);
            }
        }
        return std::move(out);
    }
}


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

const HySpexConnect::cSpectralData<float>& cHySpexCameraModel::getSpectralCalibrationPerBand() const
{
    return mSpectralCalibrationPerBand;
}

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

void cHySpexCameraModel::computePercentSaturation(bool compute)
{
    auto prevComputeState = meComputeData;
    meComputeData = compute ? eCompute::PERCENT_SATURATION : eCompute::NONE;

    if (prevComputeState != meComputeData)
        emit computeModeChanged();
}

void cHySpexCameraModel::computePercentBand(bool compute)
{
    auto prevComputeState = meComputeData;
    meComputeData = compute ? eCompute::PERCENT_BAND : eCompute::NONE;

    if (prevComputeState != meComputeData)
        emit computeModeChanged();
}

void cHySpexCameraModel::computeFocus(bool compute)
{
    auto prevComputeState = meComputeData;
    meComputeData = compute ? eCompute::FOCUS : eCompute::NONE;

    if (prevComputeState != meComputeData)
        emit computeModeChanged();
}

void cHySpexCameraModel::computeSpatialDistribution(bool compute)
{
    auto prevComputeState = meComputeData;
    meComputeData = compute ? eCompute::SPATIAL_DISTRIBUTION : eCompute::NONE;

    if (prevComputeState != meComputeData)
        emit computeModeChanged();
}

void cHySpexCameraModel::computeSpectralDistribution(bool compute)
{
    auto prevComputeState = meComputeData;
    meComputeData = compute ? eCompute::SPECTRAL_DISTRIBUTION : eCompute::NONE;

    if (prevComputeState != meComputeData)
        emit computeModeChanged();
}

std::vector<uint16_t> cHySpexCameraModel::getNumSaturated() const
{
    const std::lock_guard<std::mutex> lock(mSaturationLock);
    return mNumSaturated;
}

std::vector<float> cHySpexCameraModel::getMaxIntensity_pct() const
{
    const std::lock_guard<std::mutex> lock(mSaturationLock);
    return mMaxIntensity_pct;
}

HySpexConnect::cSpatialData<float> cHySpexCameraModel::getSpatialDistributionData() const
{
    return mSpatialDistributionData;
}

HySpexConnect::cSpectralData<float> cHySpexCameraModel::getSpectralDistributionData() const
{
    return mSpectralDistributionData;
}


void cHySpexCameraModel::postLogMessage(quint8 type, QString device, QString msg)
{
    emit logMessage(logSTATUS, device, msg);
}

void cHySpexCameraModel::computeFocusNumber(const HySpexConnect::spatial_major_data_view<uint16_t>& image)
{
    double focusNumber = 0;

    for (int b = 0; b < image.num_bands(); ++b)
    {
        auto chns = image.band(b);
        std::valarray<double> convolution_result = convolution1D(chns);
        double mean = convolution_result.sum() / convolution_result.size();
        double sd = 0.0;
        for (auto v : convolution_result)
            sd += (v - mean) * (v - mean);
        focusNumber += sd / convolution_result.size();
    }

    focusNumber = sqrt(focusNumber / image.num_bands()) / 16.0;

    emit newFocusData(focusNumber);
}

void cHySpexCameraModel::computeFocusNumber(const HySpexConnect::cSpatialMajorData<float>& image)
{
    double focusNumber = 0;

    for (int b = 0; b < image.num_bands(); ++b)
    {
        auto chns = image.band(b);
        std::valarray<double> convolution_result = convolution1D(chns);
        double mean = convolution_result.sum() / convolution_result.size();
        double sd = 0.0;
        for (auto v : convolution_result)
            sd += (v - mean) * (v - mean);
        focusNumber += sd / convolution_result.size();
    }

    focusNumber = sqrt(focusNumber / image.num_bands()) / 16.0;

    emit newFocusData(focusNumber);
}

