
#include "HySpexSWIR_384_Controller.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <cassert>

cHySpexSWIR_384_Controller::cHySpexSWIR_384_Controller(QObject* parent) : cHySpexCamera_Controller(parent)
{}

void cHySpexSWIR_384_Controller::processPacket(hyspex::ePacketType id, std::uint16_t length, const net_buffer_view& buffer)
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

void cHySpexSWIR_384_Controller::onBackgroundComplete(hyspex::BackgroundStatus status)
{
    switch(status)
    {
    case hyspex::BackgroundStatus::HYSPEX_BG_VALID:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_GOOD);
        break;
    case hyspex::BackgroundStatus::HYSPEX_BG_ABORTED:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_ABORTED);
        break;
    default:
        sendBackgroundReplyMessage(hyspex_eBackgroundReply::eBackgroundReply_FAILED);
        break;
    }
}

