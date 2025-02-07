
#include "SsnxModel.hpp"
#include "SsnxIDs.hpp"

#include <functional>


cSsnxModel::cSsnxModel(QObject* parent)
:
    cGpsModel("SSNX GPS", parent),
    mSerializer(4096)
{
}

cSsnxModel::~cSsnxModel()
{
}

const char* cSsnxModel::descriptor() const 
{
    return ssnx_id;
};

uint16_t cSsnxModel::data_class_id() const
{
    return mSerializer.classID();
}

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
        mSerializer.writeReferencePoint(mReferencePosition.avgLatitude_rad,
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

