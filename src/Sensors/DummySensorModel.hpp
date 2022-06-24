
#pragma once

#include "SensorModel.hpp"


class cDummyModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "dummy"; };

    cDummyModel(QObject* parent = nullptr);
    virtual ~cDummyModel() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader(cBlockDataFileWriter& file) override;
    void endDataRecording() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

protected:
    void update() override;
};

