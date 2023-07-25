
#pragma once

#include "HySpexVNIR_3000N_Model.hpp"
#include "Timers.hpp"

#include <cbdf/HyperspectralBuffers.hpp>
#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>

#include <memory>


// Forward Declarations


class cHySpexVNIR_3000N_Model_simulation : public cHySpexVNIR_3000N_Model
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model_simulation(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model_simulation();

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

    void setAcquisitionParameters(std::uint16_t avg_frames,
        std::uint32_t frame_period_us, std::uint32_t integration_time_us) override;

    void setNumOfBackgrounds(int num_backgrounds) override;
    void calcBackground() override;

protected:
    void update() override;

private:
    cIntervalTimer mTemperatureUpdateTimer;
    cIntervalTimer mShutterTimer;
    cIntervalTimer mBackgroundTimer;
    cIntervalTimer mSimDataUpdateTimer;

    enum class eBgStates {NONE, SH_CLOSE, COMPLETE, SH_OPEN };
    eBgStates mBackgroundState = eBgStates::NONE;
};

