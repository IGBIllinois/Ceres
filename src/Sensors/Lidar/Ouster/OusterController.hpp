
#pragma once

#include "../../SensorController.hpp"
#include "OusterControllerNetDecoder.hpp"
#include "OusterControllerNetEncoder.hpp"

class cOusterModel;

class cOusterController : public cSensorController, 
    private cOusterControllerNetDecoder, private cOusterControllerNetEncoder
{
	Q_OBJECT

public:
    cOusterController(QObject* parent = nullptr);

signals:
    void queryState();
    void queryLidarMode();
    void queryAzimuthWindow();

    void requestNewAzimuthWindow(double min_deg, double max_deg);
    void requestNewLidarMode(QString mode);

public slots:
    void stateUpdated(int mode, double min_az_deg, double max_az_deg);

    void lidarModeUpdated(int mode);
    void azimuthWindowUpdated(double min_az_deg, double max_az_deg);

protected:
    // Message handler methods from cOusterControllerNetDecoder
    void onQueryStateMessage() override;
    void onQueryLidarModeMessage() override;
    void onQueryAzimuthWindowMessage() override;
    void onSetAzimuthWindowMessage(double min_deg, double max_deg) override;
    void onSetLidarModeMessage(ouster::eLIDAR_MODE mode) override;

protected:
    /**
     * @brief processStream
     * The processStream method converts the byte stream from a TCP
     * receiver port to packet for the decoder.
     */
    void processStream(const void* pBuffer, std::size_t buf_length) override;

    /**
     * @brief sendOutgoingData
     * Sends any outgoing to the property page.
     */
    int sendOutgoingData(const char* data, std::size_t len) override
    {
        return cSensorController::sendOutgoingData(data, len);
    }
};