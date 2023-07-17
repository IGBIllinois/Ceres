
#pragma once

#include "../HyperspectralModel.hpp"
#include "../HyperspectralData.hpp"

#include <hyspex/datatypes.h>

#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>


class cHySpexVNIR_3000N_Model : public cHyperspectralModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

signals:
    void imageSizeChanged(std::size_t spatialSize, std::size_t spectralSize);


//    hyspex::CoolingStatus mCoolingStatus = hyspex::CoolingStatus::HYSPEX_COOLING_UNKNOWN;

    void avgFramesChanged(std::uint16_t avgFrames);
    void framePeriodChanged(std::uint32_t period_us);
    void minFramePeriodChanged(std::uint32_t period_us);
    void integrationTimeChanged(std::uint32_t time_us);
    void maxIntegrationTimeChanged(std::uint32_t time_us);
    void ambientTempChanged(double temp_C);
    void sensorTempChanged(double temp_C);


protected:
    void update() override;

protected:
    bool mConnected;

    unsigned int mNumBuffersRaw = 1024;
    unsigned int mNumBufferPreProcessing = 128;

    // Basic Camera Information...
    std::string mID;
    std::string mSerialNumber;
    hyspex::WavelengthRangeId mWavelengthRangeId = hyspex::WavelengthRangeId::HYSPEX_WRID_UNDEFINED;

	std::size_t mSpatialSize = 0;
    std::size_t mSpectralSize = 0;

    // This will be identical to SpatialSize/SpectralSize if no Spatial ROI is in effect.
    std::size_t mMaxSpatialSize = 0;
    std::size_t mMaxSpectralSize = 0;

    // Max pixel value, 2 ^ bpp - 1 (bits per pixel).
    unsigned short mMaxPixelValue = 0;

	hyspex::CoolingStatus mCoolingStatus = hyspex::CoolingStatus::HYSPEX_COOLING_UNKNOWN;

	std::uint16_t mAvgerageFrames = 0;
    std::uint32_t mFramePeriod_us = 0;
    std::uint32_t mMinFramePeriod_us = 0;
    std::uint32_t mIntegrationTime_us = 0;
    std::uint32_t mMaxIntegrationTime_us = 0;
	double mAmbientTemp_C = 0.0;
	double mSensorTemp_C = 0.0;

	hyspex::BackgroundStatus mBackgroundStatus = hyspex::BackgroundStatus::HYSPEX_BG_INVALID;
	cHyperspectralImageBuffer<float> mBackground;

	hyspex::AcquisitionStatus mAcquisitionStatus = hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED;

/*
	auto badPixels = vnir->getBadPixels();
	std::cout << "Num Bad Pixels = " << badPixels.size << std::endl;

	auto badCorrPixels = vnir->getBadPixelsWithCalculatedCorrections();
	std::cout << "Bad Pixels With Calculated Corrections = " << badCorrPixels.size << std::endl;

	auto badPixelsMatrix = vnir->getBadPixelsMatrix();
	std::cout << "Bad Pixels Matrix = " << badPixelsMatrix.size() << std::endl;
*/

	cHyperspectralImageBuffer<float> mResponsivityMatrix;
	cHyperspectralImageBuffer<float> mQuantumEfficiencyMatrix;

	cHyperspectralSpectralBuffer<float> SpectralCalibrationPerBand;



    cHySpexVNIR_3000N_Serializer mSerializer;
};

