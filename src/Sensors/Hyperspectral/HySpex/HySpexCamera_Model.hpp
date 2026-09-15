
#pragma once

#include "HySpexDataTypes.hpp"

#include "../../SensorInterfaces.hpp"

#include "../HyperspectralModel.hpp"
#include "../HyperspectralData.hpp"

#include <hyspex_connect/HySpexData.hpp>

#include <QObject>
#include <QImage>
#include <QStringList>

#include <vector>
#include <mutex>


class cHySpexCameraModel : public cHyperspectralModel, public iReferenceMarker
{
    Q_OBJECT
    Q_INTERFACES(iReferenceMarker)

public:
    cHySpexCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cHySpexCameraModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    /*
     * The general configuration of a HySpex hyperspectral camera.
     */
    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Retrieve the camera status variables
     */
    hyspex::InitStatus getInitStatus() const;
    hyspex::AcquisitionStatus getAcquisitionStatus() const;
    hyspex::BackgroundStatus getBackgroundStatus() const;
    hyspex::CommunicationStatus getCommunicationStatus() const;
    hyspex::CoolingStatus getCoolingStatus() const;
    hyspex::ShutterStatus getShutterStatus() const;

    /*
     * Retrieve the basic camera information
     */
    hyspex::WavelengthRangeId getWavelengthRangeId() const;

    /*
     * The current number of channels (SpatialSize) and number of 
     * spectral bands (SpectralSize) for the given region of interest (ROI).
     * These values will be identical to MaxSpatialSize/MaxSpectralSize if
     * no Spatial ROI is in effect.
     */
    std::size_t getSpatialSize() const;
    std::size_t getSpectralSize() const;

    std::size_t getMaxSpatialSize() const;
    std::size_t getMaxSpectralSize() const;

    // Max pixel value, 2 ^ bpp - 1 (bits per pixel).
    unsigned short getMaxPixelValue() const;

    const HySpexConnect::cSpectralData<float>& getSpectralCalibrationPerBand() const;

    /*
     * Retrieve the current camera information
     */
    double getAmbientTemp_C() const;
    double getSensorTemp_C() const;

    /*
     * Retrieve the current acquistion information
     */
    std::uint16_t getAverageFrames() const;
    std::uint32_t getFramePeriod_us() const;
    std::uint32_t getMinFramePeriod_us() const;
    std::uint32_t getIntegrationTime_us() const;
    std::uint32_t getMaxIntegrationTime_us() const;

// Uncomment
//    std::uint16_t getNumberFramesToSkip() const;

    virtual void setAcquisitionParameters(std::uint16_t avg_frames, 
        std::uint32_t frame_period_us, std::uint32_t integration_time_us) = 0;

// Uncomment
//    void setNumberFramesToSkip(std::uint16_t skip);

    /*
     * Retrieve the current lens information
     */
    std::string getLensName() const;
    const std::vector<std::string>& getLensNames() const;
    double getWorkingDistance_cm() const;
    double getFieldOfView_deg() const;

    /*
     * Shutter control
     */
    virtual void open_shutter() = 0;
    virtual void close_shutter() = 0;

    /*
     * Retrieve information for the background
     */
    std::uint32_t getNumOfBackgrounds() const;

    virtual void setNumOfBackgrounds(int num_backgrounds) = 0;
    virtual void calcBackground() = 0;
    virtual void stopBackground() = 0;

    /*
     * Turn on/off data computations
     */
    virtual void computePercentSaturation(bool compute);
    virtual void computePercentBand(bool compute);
    virtual void computeFocus(bool compute);
    virtual void computeSpatialDistribution(bool compute);
    virtual void computeSpectralDistribution(bool compute);

    std::vector<uint16_t> getNumSaturated() const;
    std::vector<float>    getMaxIntensity_pct() const;

    HySpexConnect::cSpatialData<float> getSpatialDistributionData() const;
    HySpexConnect::cSpectralData<float> getSpectralDistributionData() const;

signals:
    void initStatusChanged(hyspex::InitStatus status);
    void commStatusChanged(hyspex::CommunicationStatus status);
    void acqStatusChanged(hyspex::AcquisitionStatus status);
    void bgStatusChanged(hyspex::BackgroundStatus status);
    void coolingStatusChanged(hyspex::CoolingStatus status);
    void shutterStatusChanged(hyspex::ShutterStatus status);

    void imageSizeChanged(std::size_t spatialSize, std::size_t spectralSize);

    void avgFramesChanged(std::uint16_t avgFrames);
    void framePeriodChanged(std::uint32_t period_us);
    void minFramePeriodChanged(std::uint32_t period_us);
    void integrationTimeChanged(std::uint32_t time_us);
    void maxIntegrationTimeChanged(std::uint32_t time_us);
    void ambientTempChanged(double temp_C);
    void sensorTempChanged(double temp_C);

    void lensNamesChanged(QStringList lens_names);
    void lensInfoChanged(QString lens_name, double working_distance_cm, double fov_deg);

    void computeModeChanged();

    void newPercentSaturationData();
    void newPercentBandData();
    void newFocusData(double focus_number);
    void newSpatialDistributionData();
    void newSpectralDistributionData();

    void newImageData();

    void stateUpdate(int average_frames, int frame_period_us, 
        int min_frame_period_us, int integration_time_us, int max_integration_time_us,
        int num_backgrounds, QString lens_name);

public slots:
    void stateQueried();
    void lensNamesQueried();
    void shutterStateQueried();

    void requestAcquisitionParameters(int average_frame, int frame_period_us, int integration_time_us);
    void requestLensName(QString lens_name);
    void requestNumOfBackgrounds(int num_backgrounds);

    void handleCalcBackgroundRequest();
    void handleStopBackgroundRequest();

    void handleOpenShutterRequest();
    void handleCloseShutterRequest();

    virtual void onStartingReferenceMeasurement() = 0;
    virtual void onEndingReferenceMeasurement() = 0;

public:
    enum class eCompute { NONE, PERCENT_SATURATION, PERCENT_BAND, FOCUS,
        SPATIAL_DISTRIBUTION, SPECTRAL_DISTRIBUTION };

    eCompute getComputeState() const { return meComputeData; }

    void postLogMessage(quint8 type, QString device, QString msg);

protected:
    void computeFocusNumber(const HySpexConnect::spatial_major_data_view<uint16_t>& image);
    void computeFocusNumber(const HySpexConnect::cSpatialMajorData<float>& image);

protected:
    bool mConnected;

    unsigned int mNumBuffersRaw = 1024;
    unsigned int mNumBufferPreProcessing = 128;

    // Basic Camera Information...
    std::string mID;
    hyspex::WavelengthRangeId mWavelengthRangeId = hyspex::WavelengthRangeId::HYSPEX_WRID_UNDEFINED;

	std::size_t mSpatialSize = 0;
    std::size_t mSpectralSize = 0;

    // This will be identical to SpatialSize/SpectralSize if no Spatial ROI is in effect.
    std::size_t mMaxSpatialSize = 0;
    std::size_t mMaxSpectralSize = 0;

    // Max pixel value, 2 ^ bpp - 1 (bits per pixel).
    unsigned short mMaxPixelValue = 0;
    unsigned short mSaturationValue = 0;

    hyspex::InitStatus          mInitStatus = hyspex::InitStatus::HYSPEX_INIT_NOT_STARTED;
    hyspex::CommunicationStatus mCommStatus = hyspex::CommunicationStatus::HYSPEX_COMM_INIT;
    hyspex::CoolingStatus       mCoolingStatus = hyspex::CoolingStatus::HYSPEX_COOLING_UNKNOWN;
	hyspex::BackgroundStatus    mBackgroundStatus = hyspex::BackgroundStatus::HYSPEX_BG_INVALID;
    hyspex::AcquisitionStatus   mAcquisitionStatus = hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED;
    hyspex::ShutterStatus       mShutterStatus = hyspex::ShutterStatus::HYSPEX_SHUTTER_UNKNOWN;

	std::uint16_t mAverageFrames = 0;
    std::uint32_t mFramePeriod_us = 0;
    std::uint32_t mMinFramePeriod_us = 0;
    std::uint32_t mIntegrationTime_us = 0;
    std::uint32_t mMaxIntegrationTime_us = 0;

/* Uncomment
    std::uint16_t mNumFramesToSkip = 0;
    std::uint16_t mSkipNumber = 0;
*/

    double mAmbientTemp_C = 0.0;
	double mSensorTemp_C = 0.0;

    std::string mLens;
    std::vector<std::string> mLenses;
    double mWorkingDistance_cm = 0.0;
    double mFieldOfView_deg = 0.0;

    std::uint32_t mNumBackgrounds = 200;

    HySpexConnect::cSpatialMajorData<float> mBackgroundMatrix;
    HySpexConnect::cSpatialMajorData<float> mResponsivityMatrix;
    HySpexConnect::cSpectralData<float>     mQuantumEfficiencyData;
    HySpexConnect::cSpectralData<float>     mSpectralCalibrationPerBand;
    HySpexConnect::cBadPixelCorrectionData  mBadPixelCorrectionData;
    HySpexConnect::cImageData<uint16_t>     mImageData;


    enum class eCompute meComputeData = eCompute::NONE;

    mutable std::mutex mSaturationLock;

    /**
     * The number of pixels in saturation for either a spatial channel or a spectral band
     */
    std::vector<uint16_t> mNumSaturated;

    /**
     * For the spatial channel or spectral band with the greatest intensity expressed
     * as a percentage of the maximum pixel value
     */
    std::vector<float> mMaxIntensity_pct;

    int mFocusAverageCount = 0;
    const int mFocusAverageMaxCount = 10;
    HySpexConnect::cSpatialMajorData<float> mFocusMatrix;

    int mSpatialDistributionAverageCount = 0;
    const int mSpatialDistributionAverageMaxCount = 10;
    std::size_t mSpatialDistributionSpectralBand = 0;
    HySpexConnect::cSpatialData<float>  mSpatialDistWorkingData;
    HySpexConnect::cSpatialData<float>  mSpatialDistributionData;

    int mSpectralDistributionAverageCount = 0;
    const int mSpectralDistributionAverageMaxCount = 10;
    std::size_t mSpectralDistributionSpatialChannel = 0;
    HySpexConnect::cSpectralData<float> mSpectralDistWorkingData;
    HySpexConnect::cSpectralData<float> mSpectralDistributionData;

//    mpCurrentImage = new QImage(640, 480, QImage::Format_RGB888);

};

