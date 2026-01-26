
#include "TeledyneFlirCameraModel_T1300.hpp"

#define USE_LOG_MESSAGE


namespace
{
    static uint8_t teledyne_flir_t1300_instance_id = 0;
}


cTeledyneFlirCameraModel_T1300::cTeledyneFlirCameraModel_T1300(QObject* parent)
:
    cTeledyneFlirCameraModel("T1300 FLIR Camera", parent),
    mInstanceID(++teledyne_flir_t1300_instance_id)
{
    mModel = "Triton Camera";
}

cTeledyneFlirCameraModel_T1300::~cTeledyneFlirCameraModel_T1300()
{
	stopCommunications();
}

uint8_t cTeledyneFlirCameraModel_T1300::device_id() const
{
    return mInstanceID;
}

void cTeledyneFlirCameraModel_T1300::updateViews()
{
}

bool cTeledyneFlirCameraModel_T1300::configure(const nlohmann::json& jsonCfg)
{
//    size_t buffer_size = max_image_size.height * max_image_size.width;

//    mSerializer.setBufferCapacity(buffer_size + 1024);

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

void cTeledyneFlirCameraModel_T1300::enableDataRecording(cBlockDataFileWriter& file)
{
//    mSerializer.attach(&file);
}

void cTeledyneFlirCameraModel_T1300::disableDataRecording()
{
    cTeledyneFlirCameraModel::disableDataRecording();
//    mSerializer.detach();
}

void cTeledyneFlirCameraModel_T1300::writeDataHeader()
{
//    mSerializer.writeActiveCameraId(mInstanceID, mpActiveCamera->cameraID());
//    auto size = mpActiveCamera->getImageSize();
//    mSerializer.writeImageSize(mInstanceID, size.width, size.height);
//    mSerializer.writeFramesPerSecond(mInstanceID, mpActiveCamera->getFramesPerSeconds());
}

bool cTeledyneFlirCameraModel_T1300::startCommunications()
{
    if (!mConnected) return false;

    setStatus(sensor::eStatus::CONNECTING);

	return true;
}

void cTeledyneFlirCameraModel_T1300::stopCommunications()
{
    if (!mConnected) return;
    setStatus(sensor::eStatus::STOPPED);
}

void cTeledyneFlirCameraModel_T1300::errorHappend(int id, QString msg)
{
    QString full_msg = "Camera ";
    full_msg += QString::number(id);
    full_msg += ": " + msg;
    logMessage(logERROR, full_msg);

    setStatus(sensor::eStatus::FAILED);
}

