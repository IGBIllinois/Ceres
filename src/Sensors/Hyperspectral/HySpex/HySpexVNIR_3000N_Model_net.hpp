
#pragma once

#include "HySpexVNIR_3000N_Model.hpp"

#include <QObject>

class cHySpexVNIR_3000N_Model_net : public cHySpexVNIR_3000N_Model
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model_net(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model_net() = default;

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

    void setAcquisitionParameters(std::uint16_t avg_frames,
        std::uint32_t frame_period_us, std::uint32_t integration_time_us) override;

    void setNumOfBackgrounds(int num_backgrounds) override;
    void calcBackground() override;

    void open_shutter() override;
    void close_shutter() override;

signals:

protected:
    void update() override;

private:
    bool mConnected;
};

