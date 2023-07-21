
#pragma once

#include "HySpexDataTypes.hpp"

#include "../HyperspectralModel.hpp"
#include "../HyperspectralData.hpp"

#include <QObject>

#include <vector>


class cHySpexCameraModel : public cHyperspectralModel
{
    Q_OBJECT

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

    virtual void setAverageFrames(std::uint16_t frames) = 0;
    virtual void setFramePeriod_us(std::uint32_t frame_period_us) = 0;
    virtual void setIntegrationTime_us(std::uint32_t integration_time_us) = 0;

    /*
     * Retrieve the current lens information
     */
    std::string getLensName() const;
    const std::vector<std::string>& getLensNames() const;
    double getWorkingDistance_cm() const;
    double getFieldOfView_deg() const;

    /*
     * Retrieve information for the background
     */
    std::uint32_t getNumOfBackgrounds() const;

    virtual void setNumOfBackgrounds(int num_backgrounds) = 0;
    virtual void calcBackground() = 0;

signals:
    void initStatusChanged();
    void commStatusChanged();
    void acqStatusChanged();
    void bgStatusChanged();
    void coolingStatusChanged();
    void shutterStatusChanged();

    void imageSizeChanged(std::size_t spatialSize, std::size_t spectralSize);

    void avgFramesChanged(std::uint16_t avgFrames);
    void framePeriodChanged(std::uint32_t period_us);
    void minFramePeriodChanged(std::uint32_t period_us);
    void integrationTimeChanged(std::uint32_t time_us);
    void maxIntegrationTimeChanged(std::uint32_t time_us);
    void ambientTempChanged(double temp_C);
    void sensorTempChanged(double temp_C);

    void lensInfoChanged();

protected:
    static void handleStatusCallback(void* p, int eventId, int value);
    virtual void updateInitStatus(hyspex::InitStatus status);
    virtual void updateCommStatus(hyspex::CommunicationStatus status);
    virtual void updateCoolingStatus(hyspex::CoolingStatus status);
    virtual void updateBackgroundStatus(hyspex::BackgroundStatus status);
    virtual void updateAcquisitionStatus(hyspex::AcquisitionStatus status);
    virtual void updateShutterStatus(hyspex::ShutterStatus status);

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
	double mAmbientTemp_C = 0.0;
	double mSensorTemp_C = 0.0;

    std::string mLens;
    std::vector<std::string> mLenses;
    double mWorkingDistance_cm = 0.0;
    double mFieldOfView_deg = 0.0;

    std::uint32_t mNumBackgrounds = 200;

//	cHyperspectralImageBuffer<float> mBackground;

//	cHyperspectralImageBuffer<float> mResponsivityMatrix;
//	cHyperspectralImageBuffer<float> mQuantumEfficiencyMatrix;

//	cHyperspectralSpectralBuffer<float> SpectralCalibrationPerBand;
};

