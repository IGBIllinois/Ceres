
#include "LucidVisionLabsTofModel_Helios.hpp"


#define USE_LOG_MESSAGE


namespace
{
    static uint8_t lucid_helios_instance_id = 0;
}


cLucidVisionLabsTofModel_Helios::cLucidVisionLabsTofModel_Helios(QObject* parent)
:
    cLucidVisionLabsTofModel("Lucid Helios", parent),
    mInstanceID(++lucid_helios_instance_id)
{
    mModel = "Lucid Helios";
}

cLucidVisionLabsTofModel_Helios::~cLucidVisionLabsTofModel_Helios()
{
	stopCommunications();
}

uint8_t cLucidVisionLabsTofModel_Helios::device_id() const
{
    return mInstanceID;
}

void cLucidVisionLabsTofModel_Helios::updateViews()
{
}

bool cLucidVisionLabsTofModel_Helios::configure(const nlohmann::json& jsonCfg)
{
//    size_t buffer_size = max_image_size.height * max_image_size.width;

//    mSerializer.setBufferCapacity(buffer_size + 1024);

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

void cLucidVisionLabsTofModel_Helios::enableDataRecording(cBlockDataFileWriter& file)
{
//    mSerializer.attach(&file);
}

void cLucidVisionLabsTofModel_Helios::disableDataRecording()
{
    cLucidVisionLabsTofModel::disableDataRecording();
//    mSerializer.detach();
}

void cLucidVisionLabsTofModel_Helios::writeDataHeader()
{
//    mSerializer.writeActiveCameraId(mInstanceID, mpActiveCamera->cameraID());
//    auto size = mpActiveCamera->getImageSize();
//    mSerializer.writeImageSize(mInstanceID, size.width, size.height);
//    mSerializer.writeFramesPerSecond(mInstanceID, mpActiveCamera->getFramesPerSeconds());
}

bool cLucidVisionLabsTofModel_Helios::startCommunications()
{
    if (!mConnected) return false;

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cLucidVisionLabsTofModel_Helios::stopCommunications()
{
    if (!mConnected) return;

    setStatus(sensor::eStatus::STOPPED);
}


