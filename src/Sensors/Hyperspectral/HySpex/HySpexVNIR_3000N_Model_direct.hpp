
#pragma once

#include "HySpexVNIR_3000N_Model.hpp"
#include "Timers.hpp"

#include <cbdf/HyperspectralBuffers.hpp>
#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>

#include <memory>


// Forward Declarations
namespace hyspex
{
    class cVNIR3000N;
}

class cHySpexVNIR_3000N_Model_direct : public cHySpexVNIR_3000N_Model
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model_direct(std::unique_ptr<hyspex::cVNIR3000N> camera, QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model_direct();

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

signals:

protected:
    void update() override;

private:
    cIntervalTimer mTemperatureUpdateTimer;

    bool mRequestBackground = true;

    std::unique_ptr<hyspex::cVNIR3000N> mCamera;

#if 0
    /**
     * @brief getInitStatus
     * Get the current initialization status
     */
    hyspex::InitStatus getInitStatus() const;

    /**
     * @brief getCommunicationStatus
     * Get the current communication status
     */
    hyspex::CommunicationStatus getCommunicationStatus() const;

    /**
     * Get cooling status for sensor.
     */
    hyspex::CoolingStatus getCoolingStatus() const;

    /**
     * Digital cut off for sensor
     */
    unsigned int getDigitalCutOff() const;

    /**
     * @brief getMinimumFramePeriod_us
     * Get the minimum frame period in micro seconds
     */
    unsigned int getMinimumFramePeriod_us() const;

    /**
     * @brief getMaxIntegrationTime_us
     * Get the maximum integration time given a specific frame period.
     * Both the integration time and period are in micro seconds.
     */
    unsigned int getMaxIntegrationTime_us(unsigned int framePeriod_us) const;

    /**
     * @brief getAmbientTemperature_C
     * Get the camera's ambient temperature in degrees C
     */
    double getAmbientTemperature_C();

    /**
     * @brief getSensorTemperature_C
     * Get the camera's sensor temperature in degrees C
     */
    double getSensorTemperature_C();

    /**
     * Get temperature compensation model (if present).
     * The data is stored as:
     *      slope * number of spectral bands + intercept * number of spectral bands
     */
    const ConstBuffer<double>& getTemperatureCompensationModel() const;

    /**
    *
     * Get bad pixel list based on settings file.
     */
     

    const ConstBuffer<int>& getBadPixels() const;

    /**
     * Get list of bad pixels with correction coordinates.
     */
    const ConstBuffer<bad_pixel_t>& getBadPixelsWithCalculatedCorrections() const;

    /**
     * Get Matrix that is 1 for bad pixels, 0 for good ones.
     */
    const ConstBuffer<int>& getBadPixelsMatrix() const;

    /**
     * Get Responsivity RE matrix from settings file
     */
    hyspex::cLineBuffer mResponsivityMatrix;

    /**
     * Get Quantum Efficiency (QE) matrix from settings file
     */
    hyspex::cLineBuffer mQuantumEfficiencyMatrix;

public:
    /****************************************************************
     **                 Lens  Properties / Control                 **
     ****************************************************************/

     /**
      * Number of lenses detected for camera
      */
    unsigned int getLensCount() const;

    /**
     * Current lens Id.
     */
    unsigned int getLensId() const;

    /**
     * Returns count of mismatched / missing settings in set files (if any, should be 0).
     */
    unsigned int getLensSettingMismatchCount() const;

    /**
     * Return which setting is not matched.
     */
    std::string getLensSettingMismatchByIndex(unsigned int index) const;

    /**
     * ID is 0 ..getLensCount() - 1, this will determine which QE, RE, pixelsize etc to use.
     */
    void useLensId(unsigned int lens_id);

    /**
     * Get lens name from id.
     */
    std::string getLensNameFromId(unsigned int lens_id) const;

    /**
     * Get current lens name
     */
    std::string getLensName() const;

    /**
     * Get working distance in cm for lens.  0.0 = infinite.
     */
    double getLensWorkingDistance_cm() const;
    double getLensWorkingDistance_cm(unsigned int lens_id) const;

    /**
     * Get field of view in radians.  Will simply be (pixelsize_x * spatial size) converted
     * to radians unless field_of_view is provided in settings file.
     */
    double getLensFieldOfView_rad() const;
    double getLensFieldOfView_rad(unsigned int lens_id) const;

    /**
     * Determine if current lens is a Field of View expander.
     */
    bool getLensIsFieldOfViewExpander() const;
    bool getLensIsFieldOfViewExpander(unsigned int lens_id) const;

    /**
     * Determine if current lens has Equalization(EQ) filter.
     */
    bool getLensHasEqualizationFilter() const;
    bool getLensHasEqualizationFilter(unsigned int lens_id) const;

    /**
     * Returns the number of human readable list of errors found during initialization.
     */
    unsigned int getLensSettingErrorCount() const;

    /**
     * Return string with error message.
     */
    std::string getLensSettingErrorByIndex(unsigned int index) const;


    /****************************************************************
    **            Data Collection  Properties / Control            **
    ****************************************************************/

    /**
     * @brief getAverageFrames / setAverageFrames
     * Get/Set the current number of frames to average
     */
    unsigned short getAverageFrames() const;
    void setAverageFrames(unsigned short a_averageFrames);

    /**
     * @brief getFramePeriod_us / setFramePeriod_us
     * Get/Set the current frame period in micro seconds (not persistent after shutdown)
     */
    unsigned int getFramePeriod_us() const;
    void setFramePeriod_us(unsigned int period_us);

    /**
     * @brief getIntegrationTime_us / setIntegrationTime_us
     * Get/Set current integration time in us. (not persistent after shutdown)
     */
    unsigned int getIntegrationTime_us() const;
    void setIntegrationTime_us(unsigned int integrationTime_us);

public:
    /****************************************************************
    **              Background  Properties / Control               **
    ****************************************************************/

    /**
     * Get background status.
     */
    BackgroundStatus getBackgroundStatus();

    /**
     * Get background status and number of calculated backgrounds
     */
    BackgroundStatus getBackgroundStatus(unsigned int& calculatedBackgrounds) const;

    /**
     * Get/Set number of backgrounds to average when doing calculate background, if 0 is specified, it will be reset to default.
     *
     * Will be number_of_background from .set file for lens if not set with "setNumberOfBackgrounds"
     */
    unsigned int getNumberOfBackgrounds() const;
    void setNumberOfBackgrounds(unsigned int numOfBackgrounds);

    /**
     * Starts calculating background forever until stopped by stopCalculatingBackground() at which time background is saved.
     */
    void startCalculatingBackground();

    /**
     * Starts continuous calculation of background which is saved every a_numberOfBackgrounds.
     * Status reported with event HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED.
     * If numOfBackgrounds = 0 it behaves as startCalculatingBackground.
     */
    void startContinuousBackgroundCalculation(unsigned int numOfBackgrounds = 0);

    /**
     * Stops calculating background, and stores result.
     */
    void stopCalculatingBackground();

    /**
     * Will capture number_of_background images (if 0) with the shutter closed, and then return.
     * NB: requires started acquisition, or it will wait forever.
     */
    void calculateBackground(unsigned int timeout_ms = 0, unsigned int numOfBackgrounds = 0);

    /**
     * Will capture number_of_background images (if 0), but will return immediately,
     * call waitForCalculateBackgroundAsync() afterwards for proper cleanup.
     */
    void calculateBackgroundAsync(unsigned int timeout_ms = 0, unsigned int numOfBackgrounds = 0);

    /**
     * Will wait for completion of calculateBackgroundAsync().
     */
    void waitForCalculateBackgroundAsync();

    // Moving average background calculation

    /**
     * Set maximum number of bins to use for moving average background calculation.
     * Memory usage will be spatialSize * spectralSize * a_numberOfBins.
     */
    void setMovingAverageBackgroundCalculationMaxBins(unsigned int maxNumOfBins);

    /**
     * Return max number of bins to use for moving average background calculation.
     */
    unsigned int getMovingAverageBackgroundCalculationMaxBins() const;

    /**
     * Starts moving average calculation of background with getNumberOfBackgrounds().
     * Background will be updated either on stop or update.
     * NB: Does not calculate standard deviation.
     */
    void startMovingAverageBackgroundCalculation();

    /**
     * Stop moving average calculationand wait until timeout.
     */
    void stopMovingAverageBackgroundCalculation(unsigned int timeout_ms = 0);

    /**
     * Update moving average calculationand wait until timeout.
     */
    void updateMovingAverageBackgroundCalculation(unsigned int timeout_ms = 0);

    /**
     * Get background matrix as calculated by calculateBackground(), will be all 0.0 before calling calculateBackground().
     */
    const ConstBuffer<double>& getBackgroundMatrix() const;

    /**
     * Sets the background age limit in milliseconds, monitored by monitorBackgroundMatrixAge(...).
     */
    void setBackgroundMatrixAgeLimit(unsigned int ageLimit_ms);

    /**
     * Returns the current background age limit in milliseconds set by setBackgroundMatrixAgeLimit(...).
     * Returns a default value if not set.
     */
    unsigned int getBackgroundMatrixAgeLimit();

    /**
     * Returns the age of the most recent Background in milliseconds. Returns -1 if invalid (I.e. NOT set).
     */
    int64_t getBackgroundMatrixAge();

    /**
     * Monitors background ageand reports status through event HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED.
     */
    void monitorBackgroundMatrixAge(bool enable);

    /**
     * Get standard deviation for background matrix as calculated by calculateBackground(),
     * will be all 0.0 before calling calculateBackground().
     */
    const ConstBuffer<double>& getBackgroundStdDeviationMatrix() const;

    /**
     * Get average DN of background.  Will be 0.0 before calling calculateBackground().
     */
    double getBackgroundAverageDN() const;

    /**
     * Get average DN of background for each spatial position.
     * Will be 0.0 before calling calculateBackground().
     */
    const ConstBuffer<double>& getBackgroundAverageDNSpatial() const;

    /**
     * Set bands to use for saturation flag (all bands by default).
     */
    void setSaturationBands(unsigned int startBand, unsigned int endBand);

public:
    /****************************************************************
    **             Acquisition  Properties / Control               **
    ****************************************************************/

    /**
     * Get acquisition status.
     */
    AcquisitionStatus getAcquisitionStatus();

    /**
     * Various initialization to prepare before starting Acquisition
     * Set Integration time of sensor, set frame period to 0 and others
     */
    void initAcquisition();

    /**
     * Call this before startAcquisition() if it is required that
     * startAcquisition must happen ASAP.
     */
    void preAcquisition();

    /**
     * Check this to ensure that startAcquisition() happens ASAP.
     */
    void waitForPreAcquisitionComplete(unsigned int timeout_ms = 0);

    /**
     * Starts trigger with frame period and starts streaming data.
     */
    void startAcquisition(unsigned int timeout_ms = 0);

    /**
     * Will start acquisition, but will not verify that data is actually
     * received.  Only supported if using external trigger.
     */
    void startAcquisitionNoWait();

    /**
     * Stops triggerand stops streaming data.
     */
    void stopAcquisition();

    /**
     * Get next image from camera, this will keep an index for each thread
     * that calls this function, so that several threads can read from the
     * same camera, but at different speeds.
     */
    const ImageLine<unsigned short>& getNextImage(ImageOptions options = HYSPEX_RAW, uint32_t timeout_ms = 0);

    /**
     * Get next image from camera, this will keep an index for each thread that
     * calls this function, so that several threads can read from the same camera,
     * but at different speeds.  Writer will stop if reading too slow.
     */
    const ImageLine<unsigned short>& getNextImageCritical(ImageOptions options = HYSPEX_RAW, uint32_t timeout_ms = 0);

    /**
     * Release image for current thread, should only be used before exiting thread.
     */
    void releaseImage();

    /**
     * Apply matrix corrections on a buffer at rawInput with size image_size.
     */
    void applyMatrixCorrections(unsigned short* rawInput, const int image_size,
        ImageOptions options = HYSPEX_RE, bool removeBadPixels = true);



public:
    /****************************************************************
     ***                  High SNR Mode Control                   ***
     ****************************************************************/

     /**
      * Start High SNR mode(HYSPEX_HSNR_RAW)
      */
    void startHighSNRMode();

    /**
     * Grab one HighSNRImage.
     */
    void grabHighSNRImage(unsigned short averageFrames);

    /**
     * Wait for one highSNRImage complete.
     */
    void waitForHighSNRImageComplete(unsigned int timeout_ms = 0);

    /**
     * Stop High SNR mode
     */
    void stopHighSNRMode();


#endif

};

