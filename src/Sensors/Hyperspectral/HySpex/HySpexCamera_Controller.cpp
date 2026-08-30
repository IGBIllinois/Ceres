
#include "HySpexCamera_Controller.hpp"
#include "HySpexCamera_Model.hpp"

#include <cassert>


//cHySpexCamera_Controller::cHySpexCamera_Controller(cHySpexCameraModel* model, QObject* parent)
//    :
//    cSensorController(parent), mpModel(model)
//{
//    assert(mpModel);
//}

cHySpexCamera_Controller::cHySpexCamera_Controller(QObject* parent)
    :
    cSensorController(parent), cHySpexCamera_ControllerNetEncoder(512)
{
}

/*
const char* cHySpexCamera_Controller::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cHySpexCamera_Controller::manufacturer() const
{
    return mpModel->manufacturer();
}

const std::string& cHySpexCamera_Controller::model() const
{
    return mpModel->model();
}

const std::string& cHySpexCamera_Controller::serial_number() const
{
    return mpModel->serial_number();
}

const std::string& cHySpexCamera_Controller::name() const
{
    return mpModel->name();
}

const std::string& cHySpexCamera_Controller::instance() const
{
    return mpModel->instance();
}

bool cHySpexCamera_Controller::has_instance() const
{
    return mpModel->has_instance();
}
*/

void cHySpexCamera_Controller::processStream(const void* pBuffer, std::size_t buf_length)
{
    if (!pBuffer)
        return;

    net_buffer_view buffer(reinterpret_cast<const std::byte*>(pBuffer), buf_length);

    sPacketHeader_t hdr;

    while (buffer.size() > 0)
    {
        buffer >> hdr;

        if (buf_length < hdr.length)
        {
            break;
        }

        cHySpexCamera_ControllerNetDecoder::processPacket(hdr, buffer);
    }
}


/**
 * Qt slot handler 
 */
void cHySpexCamera_Controller::stateUpdated(int average_frames, int frame_period_us,
    int min_frame_period_us, int integration_time_us, int max_integration_time_us,
    int num_backgrounds, QString lens_name)
{
    sendCurrentStateMessage(true, average_frames, frame_period_us, min_frame_period_us,
        integration_time_us, max_integration_time_us, num_backgrounds, lens_name.toStdString());
}

void cHySpexCamera_Controller::lensNamesUpdated(QStringList lens_names)
{
    std::vector<std::string> names;

    for (const auto& name : lens_names)
    {
        names.push_back(name.toStdString());
    }

    sendLensNamesMessage(names);
}

void cHySpexCamera_Controller::shutterStatusUpdated(hyspex::ShutterStatus status)
{
    switch (status)
    {
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN:
        sendShutterStateReplyMessage(hyspex_eShutterState::eShutterState_OPEN);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_CLOSED:
        sendShutterStateReplyMessage(hyspex_eShutterState::eShutterState_CLOSED);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_CLOSE:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_FAIL_OPEN:
        sendShutterStateReplyMessage(hyspex_eShutterState::eShutterState_ERROR);
        break;
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_UNKNOWN:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_OPEN:
    case hyspex::ShutterStatus::HYSPEX_SHUTTER_PENDING_CLOSE:
        sendShutterStateReplyMessage(hyspex_eShutterState::eShutterState_UNKNOWN);
        break;
    }
}

void cHySpexCamera_Controller::lensInfoUpdated(QString name, double working_distance_cm, double fov_deg)
{

}


/**
 * Message Handlers
 */

void cHySpexCamera_Controller::onQueryStateMessage()
{
    emit queryState();
}

void cHySpexCamera_Controller::onQueryLensNamesMessage()
{
    emit queryLensNames();
}

void cHySpexCamera_Controller::onQueryShutterStateMessage()
{
    emit queryShutterState();
}

void cHySpexCamera_Controller::onSetAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    emit setAcquisitionParameters(average_frame, frame_period_us, integration_time_us);
}

void cHySpexCamera_Controller::onSetLensNameMessage(const std::string& lens_name)
{
    QString name = QString::fromStdString(lens_name);
    emit setLensName(name);
}

void cHySpexCamera_Controller::onSetNumOfBackgroundsMessage(int num_backgrounds)
{
    emit setNumOfBackgrounds(num_backgrounds);
}

void cHySpexCamera_Controller::onCalcBackgroundMessage()
{
    emit calcBackground();
}

void cHySpexCamera_Controller::onStopBackgroundMessage()
{
    emit stopBackground();
}

void cHySpexCamera_Controller::onOpenShutterMessage()
{
    emit openShutter();
}

void cHySpexCamera_Controller::onCloseShutterMessage()
{
    emit closeShutter();
}




