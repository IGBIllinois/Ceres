/*
 * The SsnxPropertyPage to control a GPS unit
 * connected to a remote computer.
 */

#pragma once

#include "ExperimentStateCreator.hpp"
#include "SsnxPropertyPage.hpp"
#include "../GpsPropertiesNetDecoder.hpp"
#include "../GpsPropertiesNetEncoder.hpp"


class cSsnxPropertyPage_Remote : public cSsnxPropertyPage,
    public cSensorPropertyPageRemoteInterface,
    private cGpsPropertiesNetDecoder, private cGpsPropertiesNetEncoder
{
//    Q_OBJECT

public:
    cSsnxPropertyPage_Remote(QWidget* parent = nullptr);
    explicit cSsnxPropertyPage_Remote(const std::string& instance, QWidget* parent = nullptr);
    ~cSsnxPropertyPage_Remote() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;

public:
    void onReferenceParameters(bool valid, uint16_t integration_time_sec,
        uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm) override;

    void onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
        double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid) override;

    void onReferenceCommandReply(eReferenceReply reply) override;


protected:
    void onConnect() override;
    void onDisconnect() override;

protected:
    void showPage() override;

    void doCalcReference() override;

    void doOK() override;
    void doCancel() override;
    void doApply() override;

    void reject() override;

protected:
    void processPacket(gps::ePacketType id, std::uint16_t length, const net_buffer_view& buffer) override {};

private:
    void sendChangedData();
    void queryReferenceData();
    void queryReferenceParameters();
    void setReferenceParameters(std::uint16_t min_integration_time_sec, std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm);
    void calcReference();

private:
    void decodeIncomingData(const void* pBuffer, std::size_t buf_length) override;
    int sendOutgoingData(const char* data, std::size_t len) override;

private:
    bool mReferenceParametersValid = false;
    bool mReferenceValid = false;
};


