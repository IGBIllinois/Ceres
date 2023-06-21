
#pragma once

#include "HySpexSWIR_384_Model.hpp"

#include <QObject>

class cHySpexSWIR_384_Model_direct : public cHySpexSWIR_384_Model
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model_direct(QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model_direct() = default;

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
};

