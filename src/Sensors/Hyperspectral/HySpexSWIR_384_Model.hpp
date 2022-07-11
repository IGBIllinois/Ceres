
#pragma once

#include "HyperspectralModel.hpp"
#include "../../BlockDataFile/HySpexSWIR_384_Serializer.hpp"

#include <QObject>

class cHySpexSWIR_384_Model : public cHyperspectralModel
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model(QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

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

protected:
    void update() override;

private:
    bool mConnected;

    cHySpexSWIR_384_Serializer mSerializer;
};

