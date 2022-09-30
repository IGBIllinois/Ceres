
#include "AxisCommunicationsController.hpp"
#include "AxisCommunicationsModel.hpp"
#include "AxisCommunicationsModel_F44.hpp"

#include <cassert>


cAxisCommunicationsController::cAxisCommunicationsController(cAxisCommunicationsModel* model, QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cAxisCommunicationsController::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cAxisCommunicationsController::name() const
{
    return mpModel->name();
}

void cAxisCommunicationsController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cAxisCommunicationsController::onQueryState()
{
//    auto window = mpModel->getAzimuthWindow();
//    auto mode = mpModel->getLidarMode();

//    sendCurrentState(true, mode, window.min_deg, window.max_deg);
}

/*
void cAxisCommunicationsController::onSetAzimuthWindow(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    mpModel->setAzimuthWindow(min_deg, max_deg);
}

void cAxisCommunicationsController::onSetMode(ouster::eLIDAR_MODE mode)
{
    mpModel->setLidarMode(mode);
}
*/


cAxisCommunicationsController_F44::cAxisCommunicationsController_F44(cAxisCommunicationsModel_F44* model, QObject* parent)
    :
    cSensorController(parent), cAxisControllerNetEncoder(512), mpModel(model)
{
    assert(mpModel);
}

const char* cAxisCommunicationsController_F44::descriptor() const
{
    return mpModel->descriptor();
}

const std::string& cAxisCommunicationsController_F44::name() const
{
    return mpModel->name();
}

void cAxisCommunicationsController_F44::processStream(const void* pBuffer, std::size_t buf_length)
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

void cAxisCommunicationsController_F44::onQueryState()
{
    //    auto window = mpModel->getAzimuthWindow();
    //    auto mode = mpModel->getLidarMode();

    //    sendCurrentState(true, mode, window.min_deg, window.max_deg);
}

/*
void cAxisCommunicationsController::onSetAzimuthWindow(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    mpModel->setAzimuthWindow(min_deg, max_deg);
}

void cAxisCommunicationsController::onSetMode(ouster::eLIDAR_MODE mode)
{
    mpModel->setLidarMode(mode);
}
*/

