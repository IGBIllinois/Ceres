
#pragma once

#include "LucidVisionLabsRgbModel.hpp"
#include "Timers.hpp"

#include <vector>
#include <memory>

// Forward Declaration
class cLucidTritonCamera;

namespace Arena
{
    class IImage;
}


class cLucidVisionLabsRgbModel_Triton : public cLucidVisionLabsRgbModel
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
    void enableCamera(int id);

public slots:
    void requestMode(int mode) override;
    void requestFrameRate_Hz(double frame_rate_hz) override;
    void requestFrameInterval_ms(uint32_t frame_interval_ms) override;
    void requestImage() override;
    void requestImages(bool update_view) override;

    void takePhoto(bool update_view = false) override;

protected slots:
    void errorHappend(int id, QString msg);

    virtual void processReply(const std::string& reply) {};

private:
    void updateCurrentImage(Arena::IImage* pImage);

private:
    const uint8_t mInstanceID;

    std::unique_ptr<cLucidTritonCamera> mCamera;
};

