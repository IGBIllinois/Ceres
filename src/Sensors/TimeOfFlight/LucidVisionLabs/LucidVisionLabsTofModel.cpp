
#include "LucidVisionLabsTofModel.hpp"
#include "LucidVisionLabsIDs.hpp"

#include <nlohmann/json.hpp>


cLucidVisionLabsTofModel::cLucidVisionLabsTofModel(const std::string& name, QObject* parent)
:
    cTofCameraModel(name, parent)
{
    mManufacturer = "Lucid Vision Labs";

    mConnected = false;
}

cLucidVisionLabsTofModel::~cLucidVisionLabsTofModel()
{
	stopCommunications();
}

const char* cLucidVisionLabsTofModel::descriptor() const
{
    return lucid_vision_labs_id;
}

uint16_t cLucidVisionLabsTofModel::data_class_id() const
{
    return 0; // mSerializer.classID();
}

bool cLucidVisionLabsTofModel::configure(const nlohmann::json& jsonCfg)
{
    mConnected = true;

    return cTofCameraModel::configure(jsonCfg);
}

bool cLucidVisionLabsTofModel::startCommunications()
{
	return true;
}

void cLucidVisionLabsTofModel::stopCommunications()
{
}

void cLucidVisionLabsTofModel::requestImage()
{
}

void cLucidVisionLabsTofModel::update()
{
}

