
#pragma once

#include "LucidVisionLabsTofModel.hpp"
#include "Timers.hpp"

#include <vector>


class cLucidVisionLabsTofModel_Helios : public cLucidVisionLabsTofModel
{
    Q_OBJECT

public:
    cLucidVisionLabsTofModel_Helios(QObject* parent = nullptr);
    virtual ~cLucidVisionLabsTofModel_Helios();

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Lucid Vision Labs Helios Time-of-Flight Camera
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

private:

private:
    const uint8_t mInstanceID;
};

