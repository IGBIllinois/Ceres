
#pragma once

#include "../../SensorController.hpp"
#include "HySpexCamera_ControllerNetDecoder.hpp"
#include "HySpexCamera_ControllerNetEncoder.hpp"

#include "HySpexDataTypes.hpp"



class cHySpexCamera_Controller : public cSensorController, protected cHySpexCamera_ControllerNetDecoder, protected cHySpexCamera_ControllerNetEncoder
{
	Q_OBJECT

public:
//    cHySpexCamera_Controller(cHySpexCameraModel* model, QObject* parent = nullptr);
    cHySpexCamera_Controller(QObject* parent = nullptr);

/*
public:
    const char* descriptor() const override;
    uint32_t version() const override { return 1; };
    const std::string& manufacturer() const override;
    const std::string& model() const override;
    const std::string& serial_number() const override;
    const std::string& name() const override;
    const std::string& instance() const override;
    bool has_instance() const override;
*/

signals:
    void queryState();
    void queryLensNames();
    void queryShutterState();

    void setAcquisitionParameters(int average_frame, int frame_period_us, int integration_time_us);
    void setLensName(QString lens_name);
    void setNumOfBackgrounds(int num_backgrounds);

    void calcBackground();
    void stopBackground();

    void openShutter();
    void closeShutter();

public slots:
    /*** Qt slot handler */
    void stateUpdated(int average_frames, int frame_period_us,
        int min_frame_period_us, int integration_time_us, int max_integration_time_us,
        int num_backgrounds, QString lens_name);

    void lensNamesUpdated(QStringList lens_names);
    void shutterStatusUpdated(hyspex::ShutterStatus status);
    void backgroundStatusUpdated(hyspex::BackgroundStatus status);

    void lensInfoUpdated(QString name, double working_distance_cm, double fov_deg);

    void onBackgroundComplete(hyspex::BackgroundStatus status);

protected:
    /*** Message Handlers from the network decoder */
    void onQueryStateMessage() override;
    void onQueryLensNamesMessage() override;
    void onQueryShutterStateMessage() override;
    void onSetAcquisitionParametersMessage(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us) override;
    void onSetLensNameMessage(const std::string& lens_name) override;
    void onSetNumOfBackgroundsMessage(int num_backgrounds) override;
    void onCalcBackgroundMessage() override;
    void onStopBackgroundMessage() override;

    void onOpenShutterMessage() override;
    void onCloseShutterMessage() override;

/*
protected:
    void txCurrentState(cHySpexCamera_ControllerNetEncoder* encoder);
    void txLensNames(cHySpexCamera_ControllerNetEncoder* encoder);
    void txShutterState(cHySpexCamera_ControllerNetEncoder* encoder);
*/

//private:
//    cHySpexCameraModel* mpModel = nullptr;

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


