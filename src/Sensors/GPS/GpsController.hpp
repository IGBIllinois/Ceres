
#pragma once

#include "../SensorController.hpp"
#include "GpsTypes.hpp"

#include "GpsControllerNetDecoder.hpp"
#include "GpsControllerNetEncoder.hpp"


class cGpsModel;


class cGpsController : public cSensorController,
    protected cGpsControllerNetDecoder, protected cGpsControllerNetEncoder
{
	Q_OBJECT

public:
    cGpsController(cGpsModel* pModel, QObject* parent = nullptr);

    void connectToModel() override;

public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;

signals:
    void queryReferenceState();
    void queryReferenceParameters();
    void queryReferenceData();

    void updateReferenceParameters(int min_integration_time_sec, int max_integration_time_sec, int error_threshold_mm);

    void startReferenceComputation();
    void abortReferenceCompute();

public slots:
    virtual void onReferenceComplete();
    virtual void referenceStateUpdated(::gps::eReferenceState state);

    void referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm);

    void referenceDataUpdated(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
        double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid);

    void referencePositionUpdated(int x_mm, int y_mm, int z_mm, double error_mm, int count);

    /*** Messages handlers from the decoder */
protected:
    void onQueryReferenceData() override;
    void onQueryReferenceParameters() override;
    void onQueryReferenceState() override;

    void onCalcReference() override;
    void onStopReference() override;

    void onSetReferenceParameters(std::uint16_t min_integration_time_sec,
        std::uint16_t max_integration_time_sec, std::uint16_t error_threshold_mm) override;

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

    void processPacket(gps::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override {};

protected:
    cGpsModel* mpModel = nullptr;
};


