
#pragma once

#include "HySpexSWIR_384_Model.hpp"

#include <QObject>

class cHySpexSWIR_384_Model_net : public cHySpexSWIR_384_Model
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model_net(QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model_net() = default;

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;

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
    bool mConnected;
};

