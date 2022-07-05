
#pragma once

#include "OusterModel.hpp"
#include "OusterParser.hpp"

#include <ouster/ouster_defs.h>

#include <QObject>

class cOusterModel_file : public cOusterModel
{
    Q_OBJECT

public:
    cOusterModel_file(QObject* parent = nullptr);
    virtual ~cOusterModel_file() = default;

    bool configure(const nlohmann::json& jsonCfg) override;

    void writeDataHeader(cBlockDataFileWriter& file) override {};
    void endDataRecording() override {};

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

protected:
    void onNewData(const ouster::imu_data_t& new_data) override;
    void onNewData(uint16_t frameID, const cOusterLidarData& data) override;

protected:
    void update() override;

private:
    cOusterParser mParser;
};

