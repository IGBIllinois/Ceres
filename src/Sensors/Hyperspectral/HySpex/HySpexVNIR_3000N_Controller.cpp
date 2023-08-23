
#include "HySpexVNIR_3000N_Controller.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <cassert>

//#define LOG_MESSAGE

cHySpexVNIR_3000N_Controller::cHySpexVNIR_3000N_Controller(cHySpexVNIR_3000N_Model* model, QObject* parent)
    :
    cHySpexCamera_Controller(model, parent), cHySpexVNIR_3000N_ControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

void cHySpexVNIR_3000N_Controller::processStream(const void* pBuffer, std::size_t buf_length)
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

void cHySpexVNIR_3000N_Controller::onBackgroundComplete()
{
    auto status = mpModel->getBackgroundStatus();
    switch(status)
    {
    case hyspex::BackgroundStatus::HYSPEX_BG_VALID:
    case hyspex::BackgroundStatus::HYSPEX_BG_PENDING_READY:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_GOOD);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_ABORTED:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_ABORTED);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_PENDING:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_PENDING);
        break;
    default:
        sendBackgroundReply(hyspex_eBackgroundReply::eBackgroundReply_FAILED);
        break;
    }
}

void cHySpexVNIR_3000N_Controller::onQueryState()
{
    txCurrentState(this);

#ifdef LOG_MESSAGE
    QString msg = "onQueryState";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onQueryLensNames()
{
    txLensNames(this);

#ifdef LOG_MESSAGE
    QString msg = "onQueryLensNames";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onQueryShutterState()
{
    txShutterState(this);

#ifdef LOG_MESSAGE
    QString msg = "onQueryShutterState";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onSetAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    mpModel->setAcquisitionParameters(average_frame, frame_period_us, integration_time_us);

#ifdef LOG_MESSAGE
    QString msg = "onSetAcquisitionParameters: ";
    msg += QString::number(average_frame);
    msg += ", ";
    msg += QString::number(frame_period_us);
    msg += ", ";
    msg += QString::number(integration_time_us);
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onSetLensName(const std::string& lens_name)
{

#ifdef LOG_MESSAGE
    QString msg = "onSetLensName: ";
    msg += QString::fromStdString(lens_name);
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onSetNumOfBackgrounds(int num_backgrounds)
{
    mpModel->setNumOfBackgrounds(num_backgrounds);

#ifdef LOG_MESSAGE
    QString msg = "onSetNumOfBackgrounds";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onCalcBackground()
{
    mpModel->calcBackground();

#ifdef LOG_MESSAGE
    QString msg = "onCalcBackground";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onStopBackground()
{
    mpModel->stopBackground();

#ifdef LOG_MESSAGE
    QString msg = "onStopBackground";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onOpenShutter()
{
    mpModel->open_shutter();

#ifdef LOG_MESSAGE
    QString msg = "onOpenShutter";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}

void cHySpexVNIR_3000N_Controller::onCloseShutter()
{
    mpModel->close_shutter();

#ifdef LOG_MESSAGE
    QString msg = "onCloseShutter";
    mpModel->postLogMessage(logSTATUS, "cHySpexVNIR_3000N_Controller", msg);
#endif
}



