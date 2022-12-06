
#pragma once

#include "HySpexVNIR_3000N_Model.hpp"

#include <QObject>

class cHySpexVNIR_3000N_Model_direct : public cHySpexVNIR_3000N_Model
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model_direct(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model_direct() = default;

    bool configure(const nlohmann::json& jsonCfg) override;

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
};

