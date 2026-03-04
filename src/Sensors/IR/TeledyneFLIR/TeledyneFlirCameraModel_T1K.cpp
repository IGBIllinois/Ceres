
#include "TeledyneFlirCameraModel_T1K.hpp"

#include <TeledyneAtlasConnect/TeledyneFlirCamera.hpp>

#define USE_LOG_MESSAGE


namespace
{
    static uint8_t teledyne_flir_t1k_instance_id = 0;
}


cTeledyneFlirCameraModel_T1K::cTeledyneFlirCameraModel_T1K(std::unique_ptr<cTeledyneFlirCamera> camera, QObject* parent)
:
    cTeledyneFlirCameraModel("T1K FLIR Camera", parent),
    mInstanceID(++teledyne_flir_t1k_instance_id)
{
    mCamera.reset(camera.release());
    mModel = mCamera->modelName();
}

cTeledyneFlirCameraModel_T1K::~cTeledyneFlirCameraModel_T1K()
{
	stopCommunications();
}

uint8_t cTeledyneFlirCameraModel_T1K::device_id() const
{
    return mInstanceID;
}

void cTeledyneFlirCameraModel_T1K::updateViews()
{
}

bool cTeledyneFlirCameraModel_T1K::configure(const nlohmann::json& jsonCfg)
{
//    size_t buffer_size = max_image_size.height * max_image_size.width;

//    mSerializer.setBufferCapacity(buffer_size + 1024);

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

void cTeledyneFlirCameraModel_T1K::enableDataRecording(cBlockDataFileWriter& file)
{
//    mSerializer.attach(&file);
}

void cTeledyneFlirCameraModel_T1K::disableDataRecording()
{
    cTeledyneFlirCameraModel::disableDataRecording();
//    mSerializer.detach();
}

void cTeledyneFlirCameraModel_T1K::writeDataHeader()
{
//    mSerializer.writeActiveCameraId(mInstanceID, mpActiveCamera->cameraID());
//    auto size = mpActiveCamera->getImageSize();
//    mSerializer.writeImageSize(mInstanceID, size.width, size.height);
//    mSerializer.writeFramesPerSecond(mInstanceID, mpActiveCamera->getFramesPerSeconds());
}

bool cTeledyneFlirCameraModel_T1K::startCommunications()
{
    if (!mConnected) return false;

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cTeledyneFlirCameraModel_T1K::stopCommunications()
{
    if (!mConnected) return;
    setStatus(sensor::eStatus::STOPPED);
}

void cTeledyneFlirCameraModel_T1K::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

