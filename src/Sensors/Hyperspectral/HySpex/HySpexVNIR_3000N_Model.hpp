
#pragma once

#include "HySpexCamera_Model.hpp"

#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>


class cHySpexVNIR_3000N_Model : public cHySpexCameraModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model();

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: HySpex VNIR 3000N Hyperspectral Camera
     */
    uint8_t device_id() const override;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    void writeDataHeader() override;

signals:
    void backgroundComplete(hyspex::BackgroundStatus status);

protected:
    void onStartingReferenceMeasurement() override;
    void onEndingReferenceMeasurement() override;

protected:
    cHySpexVNIR_3000N_Serializer mSerializer;

private:
    const uint8_t mDeviceID;
};

