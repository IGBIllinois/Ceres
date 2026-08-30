
#pragma once

#include "HySpexSWIR_384_Model.hpp"
#include "Timers.hpp"

#include <cbdf/HySpexSWIR_384_Serializer.hpp>

#include <QObject>

#include <memory>


// Forward Declarations
namespace hyspex
{
    class cSWIR384;
}

class cHySpexSWIR_384_Model_direct : public cHySpexSWIR_384_Model
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model_direct(std::unique_ptr<hyspex::cSWIR384> camera, QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model_direct();

    /*
     * Emit all status messages to update all views
     */
    void updateViews() override;

    bool configure(const nlohmann::json& jsonCfg) override;
    bool initialize() override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    void writeDataHeader() override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void setAcquisitionParameters(std::uint16_t avg_frames,
        std::uint32_t frame_period_us, std::uint32_t integration_time_us) override;

    void setNumOfBackgrounds(int num_backgrounds) override;
    void calcBackground() override;
    void stopBackground() override;

    void open_shutter() override;
    void close_shutter() override;

    /*
     * Turn on/off data computations
     */
    void computePercentSaturation(bool compute) override;
    void computePercentBand(bool compute) override;
    void computeFocus(bool compute) override;
    void computeSpatialDistribution(bool compute) override;
    void computeSpectralDistribution(bool compute) override;

/*
 * Status Callback Methods
 */
protected:
    static void handleStatusCallback(void* p, int eventId, int value);
    void updateInitStatus(hyspex::InitStatus status);
    void updateCommStatus(hyspex::CommunicationStatus status);
    void updateCoolingStatus(hyspex::CoolingStatus status);
    void updateBackgroundStatus(hyspex::BackgroundStatus status);
    void updateAcquisitionStatus(hyspex::AcquisitionStatus status);
    void updateShutterStatus(hyspex::ShutterStatus status);

/*
 * Image Callback Method
 */
protected:
    static void handleImageCallback(void* p, hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image);
    void updateImageData(hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image);

/*
 * Sensor Method Update
 */
protected:
    void update() override;


private:
    cIntervalTimer mTemperatureUpdateTimer;

    enum class eBgStates {NONE, SH_CLOSE, STARTED, SH_OPEN, ABORT };
    Q_ENUM(eBgStates)

    eBgStates mBgCurrentState = eBgStates::NONE;

    std::unique_ptr<hyspex::cSWIR384> mCamera;
};

