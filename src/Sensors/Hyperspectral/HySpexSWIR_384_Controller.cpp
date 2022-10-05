
#include "HySpexSWIR_384_Controller.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <cassert>


cHySpexSWIR_384_Controller::cHySpexSWIR_384_Controller(cHySpexSWIR_384_Model* model, QObject* parent)
    :
    cSensorController(parent), cHySpexSWIR_384_ControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cHySpexSWIR_384_Controller::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cHySpexSWIR_384_Controller::name() const
{
    return mpModel->name();
}

void cHySpexSWIR_384_Controller::processStream(const void* pBuffer, std::size_t buf_length)
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

        processPacket(hdr, buffer);
    }
}

void cHySpexSWIR_384_Controller::onQueryState()
{
//    auto window = mpModel->getAzimuthWindow();
//    auto mode = mpModel->getLidarMode();

//    sendCurrentState(true, mode, window.min_deg, window.max_deg);
}


