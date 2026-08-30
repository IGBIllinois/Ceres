
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

void cHySpexVNIR_3000N_Controller::onBackgroundComplete(hyspex::BackgroundStatus status)
{
    switch(status)
    {
    case hyspex::BackgroundStatus::HYSPEX_BG_VALID:
    case hyspex::BackgroundStatus::HYSPEX_BG_PENDING_READY:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_GOOD);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_ABORTED:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_ABORTED);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_PENDING:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_PENDING);
        break;
    default:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_FAILED);
        break;
    }
}

