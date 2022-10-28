
#pragma once

#include "../SensorController.hpp"
#include "OusterControllerNetDecoder.hpp"
#include "OusterControllerNetEncoder.hpp"

class cOusterModel;

class cOusterController : public cSensorController, 
    private cOusterControllerNetDecoder, private cOusterControllerNetEncoder
{
	Q_OBJECT

public:
    cOusterController(cOusterModel* model, QObject* parent = nullptr);

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;

protected:
    void onQueryState() override;
    void onQueryLidarMode() override;
    void onQueryAzimuthWindow() override;
    void setAzimuthWindow(double min_deg, double max_deg) override;
    void setLidarMode(ouster::eLIDAR_MODE mode) override;

signals:
    void requestNewAzimuthWindow(double min_deg, double max_deg);
//    void requestNewLidarMode(ouster::eLIDAR_MODE mode);
    void requestNewLidarMode(QString mode);

public slots:
    void azimuthWindowChanged();
    void dataFormatChanged();

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

private:
    cOusterModel* mpModel = nullptr;
};