
#include "SsnxModel.hpp"
#include "SsnxFactory.hpp"

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

void cSsnxModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSsnxModel::disableDataRecording()
{
    cGpsModel::disableDataRecording();
    mSerializer.detach();
}

