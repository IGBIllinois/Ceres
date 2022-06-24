
#include "DummySensorModel.hpp"
#include "DummySensorFactory.hpp"


cDummyModel::cDummyModel(QObject* parent)
:
	cSensorModel(parent)
{
}


char* cDummyModel::descriptor() const
{
    return dummy_id;
}

bool cDummyModel::configure(const nlohmann::json& jsonCfg)
{

    return true;
}

bool cDummyModel::startCommunications()
{
    return true;
}

void cDummyModel::stopCommunications()
{
}

void cDummyModel::update()
{
}

void cDummyModel::writeDataHeader(cBlockDataFileWriter& file)
{
}

void cDummyModel::endDataRecording()
{
}



