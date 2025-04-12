
#include "HokuyoModel.hpp"
#include "HokuyoIDs.hpp"
#include "Constants.hpp"

#include <optional>


namespace
{
    static uint8_t hokuyo_device_id = 0;
}


cHokuyoModel::cHokuyoModel(QObject* parent)
:
    cLidarModel("Hokuyo", parent), mDeviceID(++hokuyo_device_id)
{
    mManufacturer = "Hokuyo";
    mFrameCounter = 0;
}

uint8_t cHokuyoModel::device_id() const
{
    return mDeviceID;
}

const char* cHokuyoModel::descriptor() const
{
    return hokuyo_id;
}

