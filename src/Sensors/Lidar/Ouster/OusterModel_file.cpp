
#include "OusterModel_file.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cOusterModel_file::cOusterModel_file(QObject* parent)
:
    cOusterModel(parent)
{
}

bool cOusterModel_file::configure(const nlohmann::json& jsonCfg)
{
    return true;
}

bool cOusterModel_file::startCommunications()
{
    return true;
}

void cOusterModel_file::stopCommunications()
{
}

void cOusterModel_file::update()
{
}

void cOusterModel_file::onNewData(const ouster::imu_data_t& data)
{
}

void cOusterModel_file::onNewData(uint16_t frameID, const cOusterLidarData& data)
{
}

