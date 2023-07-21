
#pragma once

#include "HySpexVNIR_3000N_Model.hpp"
#include "Timers.hpp"

#include <cbdf/HyperspectralBuffers.hpp>
#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>

#include <memory>


// Forward Declarations
namespace hyspex
{
    class cVNIR3000N;
}

class cHySpexVNIR_3000N_Model_direct : public cHySpexVNIR_3000N_Model
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model_direct(std::unique_ptr<hyspex::cVNIR3000N> camera, QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model_direct();

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

    void setAverageFrames(std::uint16_t frames) override;
    void setFramePeriod_us(std::uint32_t frame_period_us) override;
    void setIntegrationTime_us(std::uint32_t integration_time_us) override;

    void setNumOfBackgrounds(int num_backgrounds) override;
    void calcBackground() override;

signals:

protected:
    void update() override;

private:
    cIntervalTimer mTemperatureUpdateTimer;

    bool mRequestBackground = true;

    std::unique_ptr<hyspex::cVNIR3000N> mCamera;
};

