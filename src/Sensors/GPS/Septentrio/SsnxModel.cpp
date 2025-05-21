
#include "SsnxModel.hpp"
#include "SsnxIDs.hpp"

#include <functional>


namespace
{
    static uint8_t ssnx_device_id = 0;
}


cSsnxModel::cSsnxModel(QObject* parent)
:
    cGpsModel("SSNX GPS", parent),
    mSerializer(4096),
    mDeviceID(++ssnx_device_id)
{
}

cSsnxModel::cSsnxModel(const std::string& instance, QObject* parent)
    :
    cGpsModel("SSNX GPS", instance, parent),
    mSerializer(4096),
    mDeviceID(++ssnx_device_id)
{}


cSsnxModel::~cSsnxModel()
{
}

const char* cSsnxModel::descriptor() const
{
    return ssnx_id;
};

bool cSsnxModel::configure(const nlohmann::json& jsonCfg)
{
    auto result = cGpsModel::configure(jsonCfg);

    if (!mModel.empty())
        updateName(mModel);

    return result;
}

void cSsnxModel::dataRecordingStateChange(bool record)
{
    if ((mReferenceState == ::gps::eReferenceState::COMPLETE_GOOD) && mReferencePosition.valid
        && static_cast<bool>(mSerializer))
    {
        mSerializer.writeReferencePoint(device_id(), mReferencePosition.avgLatitude_rad,
            mReferencePosition.avgLongitude_rad, mReferencePosition.avgHeight_m,
            mReferencePosition.stdLatitude_rad, mReferencePosition.stdLongitude_rad, mReferencePosition.stdHeight_m,
            mReferencePosition.heightValid);

        mReferencePosition.valid = false;
        mReferenceState = ::gps::eReferenceState::WAITING;
    }

    cGpsModel::dataRecordingStateChange(record);
}

void cSsnxModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSsnxModel::disableDataRecording()
{
    cGpsModel::disableDataRecording();
    mSerializer.detach();
}

