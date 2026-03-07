
#pragma once

#include "TeledyneFlirCameraModel.hpp"
#include "Timers.hpp"
#include "ColorTable.hpp"

#include <cbdf/TeledyneFlirSerializer.hpp>

#include <vector>
#include <memory>
#include <optional>

// Forward Declarations
class cTeledyneFlirCamera;


class cTeledyneFlirCameraModel_T1K : public cTeledyneFlirCameraModel
{
    Q_OBJECT

public:
    cTeledyneFlirCameraModel_T1K(std::unique_ptr<cTeledyneFlirCamera> camera, QObject* parent = nullptr);
    virtual ~cTeledyneFlirCameraModel_T1K();

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Teledyne FLIR T1K Camera
     */
    uint8_t device_id() const override;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;

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

public slots:

protected slots:
    void errorHappend(int id, QString msg);

    virtual void processReply(const std::string& reply) {};

protected:
    bool updateFrameInterval(uint32_t frame_interval_ms) override;
    bool updateFrameRate(double frame_rate_fps) override;

    void update() override;

protected:
    cColorTable mColorTable;

private:
    std::unique_ptr<cTeledyneFlirCamera> mCamera;

    bool mIsRunning = false;

    cTeledyneFlirSerializer mSerializer;

private:
    const uint8_t mInstanceID;
};

