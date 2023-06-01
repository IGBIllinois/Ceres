
#include "HokuyoModel.hpp"
#include "HokuyoFactory.hpp"
#include "Constants.hpp"

#include <optional>

cHokuyoModel::cHokuyoModel(QObject* parent)
:
    cLidarModel("Hokuyo", parent)
{
    mManufacturer = "Hokuyo";
    mFrameCounter = 0;
}

const char* cHokuyoModel::descriptor() const
{
    return hokuyo_id;
}

