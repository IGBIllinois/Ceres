
#include "DummySensorModel.hpp"
#include "DummySensorIDs.hpp"


namespace
{
    static uint8_t dummy_id = 0;
}


cDummyModel::cDummyModel(QObject* parent)
:
	cSensorModel("Dummy", parent), mDeviceID(++dummy_id)
{
}

uint8_t cDummyModel::device_id() const
{
    return mDeviceID;
}

const char* cDummyModel::descriptor() const
{
    return dummy_class_id;
}

uint16_t cDummyModel::data_class_id() const
{
    return 0;
}

void cDummyModel::updateViews()
{
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




