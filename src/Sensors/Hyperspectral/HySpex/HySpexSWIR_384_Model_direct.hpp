
#pragma once

#include "HySpexSWIR_384_Model.hpp"
#include "Timers.hpp"

#include <QObject>

#include <memory>


// Forward Declarations
namespace hyspex
{
    class cSWIR384;
}

class cHySpexSWIR_384_Model_direct : public cHySpexSWIR_384_Model
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model_direct(std::unique_ptr<hyspex::cSWIR384> camera, QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model_direct();

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

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
    cIntervalTimer mTemperatureUpdateTimer;

    std::unique_ptr<hyspex::cSWIR384> mCamera;
};

