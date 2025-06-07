
#include "LucidVisionLabsRgbModel.hpp"
#include "LucidVisionLabsIDs.hpp"

#include <nlohmann/json.hpp>

cLucidVisionLabsRgbModel::cLucidVisionLabsRgbModel(const std::string& name, QObject* parent)
:
    cRgbCameraModel(name, parent)
{
    mManufacturer = "Lucid Vision Labs";

    mConnected = false;
}

cLucidVisionLabsRgbModel::~cLucidVisionLabsRgbModel()
{
	stopCommunications();
}

const char* cLucidVisionLabsRgbModel::descriptor() const
{
    return lucid_vision_labs_id;
}

uint16_t cLucidVisionLabsRgbModel::data_class_id() const
{
    return 0; // mSerializer.classID();
}

bool cLucidVisionLabsRgbModel::configure(const nlohmann::json& jsonCfg)
{
    mConnected = true;

    return cRgbCameraModel::configure(jsonCfg);
}

bool cLucidVisionLabsRgbModel::startCommunications()
{
    return true;
}

void cLucidVisionLabsRgbModel::stopCommunications()
{
}

void cLucidVisionLabsRgbModel::requestImage()
{
}

void cLucidVisionLabsRgbModel::update()
{
}

