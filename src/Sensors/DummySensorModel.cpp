
#include "DummySensorModel.hpp"
#include "DummySensorFactory.hpp"


cDummyModel::cDummyModel(QObject* parent)
:
	cSensorModel("Dummy", parent)
{
}

const char* cDummyModel::descriptor() const
{
    return dummy_id;
}

uint16_t cDummyModel::data_class_id() const
{
    return 0;
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

void cDummyModel::enableDataRecording(cBlockDataFileWriter& file)
{
}

void cDummyModel::disableDataRecording()
{}

void cDummyModel::writeDataHeader()
{}




