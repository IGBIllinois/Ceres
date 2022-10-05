
#include "HySpexVNIR_3000N_Controller.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <cassert>


cHySpexVNIR_3000N_Controller::cHySpexVNIR_3000N_Controller(cHySpexVNIR_3000N_Model* model, QObject* parent)
    :
    cSensorController(parent), cHySpexVNIR_3000N_ControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cHySpexVNIR_3000N_Controller::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cHySpexVNIR_3000N_Controller::name() const
{
    return mpModel->name();
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

        processPacket(hdr, buffer);
    }
}

/*
void cHySpexVNIR_3000N_Controller::onQueryState()
{
}
*/

