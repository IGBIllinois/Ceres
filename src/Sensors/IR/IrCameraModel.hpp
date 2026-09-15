
#pragma once

#include "../SensorModel.hpp"

#include "ThermalColorTypes.hpp"


// Forward Declaration
class cThermalColorTable;


class cIrCameraModel : public cSensorModel
{
    Q_OBJECT

public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: IR Camera
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "ir"; };

    void updateViews() override;

signals:
    void colorModelUpdated(eColorTable color_table);

public slots:
    void setColorModel(eColorTable color_table);

protected:
    cIrCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cIrCameraModel() = default;

protected:
    std::unique_ptr<cThermalColorTable> mColorTable;

private:
    const uint8_t mDeviceID;
};

