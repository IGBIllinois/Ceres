
#pragma once

#include "HyperspectralModel.hpp"
#include "HySpexVNIR_3000N_Serializer.hpp"

#include <QObject>

class cHySpexVNIR_3000N_Model : public cHyperspectralModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model();
    virtual ~cHySpexVNIR_3000N_Model() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader(cBlockDataFile& file) override;
    void endDataRecording() override;

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

    cHySpexVNIR_3000N_Serializer mSerializer;
};

