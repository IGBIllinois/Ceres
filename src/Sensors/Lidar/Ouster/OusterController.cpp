
#include "OusterController.hpp"
#include "OusterModel.hpp"

#include <ouster_connect/ouster_utils.h>

#include <cassert>


cOusterController::cOusterController(QObject* parent)
    :
    cSensorController(parent), cOusterControllerNetEncoder(512)
{}

void cOusterController::processStream(const void* pBuffer, std::size_t buf_length)
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

void cOusterController::stateUpdated(int mode, double min_az_deg, double max_az_deg)
{
    ouster::eLIDAR_MODE lidar_mode = static_cast<ouster::eLIDAR_MODE>(mode);

    sendCurrentStateMessage(true, lidar_mode, min_az_deg, max_az_deg);
}

void cOusterController::lidarModeUpdated(int mode)
{
    ouster::eLIDAR_MODE lidar_mode = static_cast<ouster::eLIDAR_MODE>(mode);

    sendLidarModeMessage(lidar_mode);
}

void cOusterController::azimuthWindowUpdated(double min_az_deg, double max_az_deg)
{
    sendAzimuthWindowMessage(min_az_deg, max_az_deg);
}


void cOusterController::onQueryStateMessage()
{
    emit queryState();
}

void cOusterController::onQueryLidarModeMessage()
{
    emit queryLidarMode();
}

void cOusterController::onQueryAzimuthWindowMessage()
{
    emit queryAzimuthWindow();
}

void cOusterController::onSetAzimuthWindowMessage(double min_deg, double max_deg)
{
    if (max_deg < min_deg)
        std::swap(min_deg, max_deg);

    emit requestNewAzimuthWindow(min_deg, max_deg);
}

void cOusterController::onSetLidarModeMessage(ouster::eLIDAR_MODE mode)
{
    emit requestNewLidarMode(QString::fromStdString(to_string(mode)));
}


