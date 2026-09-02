
#include "HySpexVNIR_3000N_Controller.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <cassert>

//#define LOG_MESSAGE

cHySpexVNIR_3000N_Controller::cHySpexVNIR_3000N_Controller(QObject* parent) : cHySpexCamera_Controller(parent)
{
}

void cHySpexVNIR_3000N_Controller::processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer)
{
    using namespace hyspex;

    switch (id)
    {
    default:
    {
        break;
    }
    }
}


