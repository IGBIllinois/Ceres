
#pragma once

#include "LucidVisionLabsRgbModel.hpp"
#include "Timers.hpp"

#include <LucidVisionLabsConnect/LucidVisionLabsTypes.hpp>

#include <Arena/ArenaAPI.h>

#include <vector>
#include <memory>
#include <optional>

// Forward Declaration
class cLucidTritonCamera;

namespace Arena
{
    class IImage;
}


class cLucidVisionLabsRgbModel_Triton : public cLucidVisionLabsRgbModel, public Arena::IImageCallback
{
    Q_OBJECT

public:
    cLucidVisionLabsRgbModel_Triton(std::unique_ptr<cLucidTritonCamera> camera, QObject* parent = nullptr);
    virtual ~cLucidVisionLabsRgbModel_Triton();

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Axis Communications F44 RGB Camera
     */
    uint8_t device_id() const override;

    void setMode(eMode mode) override;
    void setFrameRate_Hz(double frame_rate_hz) override;
    void setLapseInterval_ms(uint32_t interval_ms) override;

    /**
     * @brief Sets the automatic exposure mode.
     */
    nLucidVisionLabsConnect::nTriton::eExposureAuto exposureAuto() const;
    bool exposureAuto(nLucidVisionLabsConnect::nTriton::eExposureAuto mode);

    /**
     * @brief Controls the device exposure time in microseconds (us).
     */
    double exposureTime_us() const;
    bool exposureTime_us(double time);

    /**
     * @brief Selects which exposure time is controlled by the Exposure Time feature. This allows for independent control over the exposure components.
     */
    nLucidVisionLabsConnect::nTriton::eExposureTimeSelector exposureTimeSelector() const;
    bool exposureTimeSelector(nLucidVisionLabsConnect::nTriton::eExposureTimeSelector mode);

    /**
    * @brief Controls the mode for automatic white balancing between the color channels. The white balancing ratios are automatically adjusted.
    */
    nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto balanceWhiteAuto() const;
    bool balanceWhiteAuto(nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto mode);

    /**
     * @brief Controls the selected Gain as an absolute physical value.
     */
    float gain_dB() const;
    bool gain_dB(float level_dB);

    /**
    * @brief Sets the automatic gain control mode.
    */
    nLucidVisionLabsConnect::nTriton::eGainAuto gainAuto() const;
    bool gainAuto(nLucidVisionLabsConnect::nTriton::eGainAuto mode);

    /**
     * @brief Controls the gamma correction of pixel intensity.
     */
    float gamma() const;
    bool gamma(float level);

    /**
     * @brief Controls the selected balance ratio as an absolute physical value. This is an amplification factor applied to the video signal.
     */
    bool gammaEnable() const;
    bool gammaEnable(bool enable);

    /**
    * @brief Sets the pixel format used by the camera when sending data.
    */
    nLucidVisionLabsConnect::nTriton::ePixelFormat pixelFormat() const;
    bool pixelFormat(nLucidVisionLabsConnect::nTriton::ePixelFormat mode);

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    /*
     * Attach/Detach the serializer.
     */
    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void update() override;

signals:
    void pixelFormatChanged(int mode);
    void exposureChanged(int mode, double exposureTime_us);
    void gainChanged(int mode, double gain_dB);
    void balanceWhiteAutoChanged(int mode);
    void gammaChanged(bool enabled, double gamma);

public slots:
    void pushStreamState();
    void popStreamState();
    void requestPixelFormat(int mode);
    void requestExposure(int mode, double exposureTime_us);
    void requestGain(int mode, double gain_dB);
    void requestBalanceWhiteAuto(int mode);
    void requestGamma(bool enable, double gamma);

public slots:
    void requestMode(int mode) override;
    void requestFrameRate_Hz(double frame_rate_hz) override;
    void requestLapseInterval_ms(uint32_t interval_ms) override;
    void requestImage() override;
    void requestImages(bool update_view) override;

    void takePhoto(bool update_view = false) override;

protected slots:
    void errorHappend(int id, QString msg);

    virtual void processReply(const std::string& reply) {};

private:
    void OnImage(Arena::IImage* pImage) override;

private:
    void updateCurrentImage(Arena::IImage* pImage);

private:
    const uint8_t mInstanceID;

    std::optional<bool> mStreamStateStack;

    nLucidVisionLabsConnect::nTriton::ePixelFormat mPixelFormat = nLucidVisionLabsConnect::nTriton::ePixelFormat::BayerRG16;

    double mExposureTime_us = 0.0;
    nLucidVisionLabsConnect::nTriton::eExposureAuto mExposureAuto = nLucidVisionLabsConnect::nTriton::eExposureAuto::CONTINUOUS;

    nLucidVisionLabsConnect::nTriton::eGainAuto mGainAuto = nLucidVisionLabsConnect::nTriton::eGainAuto::CONTINUOUS;
    double mGain_dB = 0.0;

    nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto mBalanceWhiteAuto = nLucidVisionLabsConnect::nTriton::eBalanceWhiteAuto::CONTINUOUS;

    bool mGammaEnable = false;
    double mGamma = 1.0;

    std::unique_ptr<cLucidTritonCamera> mCamera;
};

