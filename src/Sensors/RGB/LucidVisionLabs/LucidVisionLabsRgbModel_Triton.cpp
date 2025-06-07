
#include "LucidVisionLabsRgbModel_Triton.hpp"

#define USE_LOG_MESSAGE


namespace
{
    static uint8_t lucid_vision_labs_triton_instance_id = 0;
}


cLucidVisionLabsRgbModel_Triton::cLucidVisionLabsRgbModel_Triton(QObject* parent)
:
    cLucidVisionLabsRgbModel("Triton Camera", parent),
    mInstanceID(++lucid_vision_labs_triton_instance_id)
{
    mModel = "Triton Camera";
}

cLucidVisionLabsRgbModel_Triton::~cLucidVisionLabsRgbModel_Triton()
{
	stopCommunications();
}

uint8_t cLucidVisionLabsRgbModel_Triton::device_id() const
{
    return mInstanceID;
}

void cLucidVisionLabsRgbModel_Triton::updateViews()
{
}

bool cLucidVisionLabsRgbModel_Triton::configure(const nlohmann::json& jsonCfg)
{
//    size_t buffer_size = max_image_size.height * max_image_size.width;

//    mSerializer.setBufferCapacity(buffer_size + 1024);

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

void cLucidVisionLabsRgbModel_Triton::enableDataRecording(cBlockDataFileWriter& file)
{
//    mSerializer.attach(&file);
}

void cLucidVisionLabsRgbModel_Triton::disableDataRecording()
{
    cLucidVisionLabsRgbModel::disableDataRecording();
//    mSerializer.detach();
}

void cLucidVisionLabsRgbModel_Triton::writeDataHeader()
{
//    mSerializer.writeActiveCameraId(mInstanceID, mpActiveCamera->cameraID());
//    auto size = mpActiveCamera->getImageSize();
//    mSerializer.writeImageSize(mInstanceID, size.width, size.height);
//    mSerializer.writeFramesPerSecond(mInstanceID, mpActiveCamera->getFramesPerSeconds());
}

bool cLucidVisionLabsRgbModel_Triton::startCommunications()
{
    if (!mConnected) return false;

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cLucidVisionLabsRgbModel_Triton::stopCommunications()
{
    if (!mConnected) return;
    setStatus(sensor::eStatus::STOPPED);
}

void cLucidVisionLabsRgbModel_Triton::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

