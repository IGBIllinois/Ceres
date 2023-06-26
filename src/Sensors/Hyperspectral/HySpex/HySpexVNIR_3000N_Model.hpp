
#pragma once

#include "../HyperspectralModel.hpp"
#include "../HyperspectralData.hpp"

#include <hyspex/datatypes.h>

#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>


class cHySpexVNIR_3000N_Model : public cHyperspectralModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model() = default;

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

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

signals:

protected:
    void update() override;

protected:
    bool mConnected;

    unsigned int mNumBuffersRaw = 1024;
    unsigned int mNumBufferPreProcessing = 128;

    // Basic Camera Information...
    std::string mID;
    std::string mSerialNumber;
    hyspex::WavelengthRangeId mWavelengthRangeId = hyspex::WavelengthRangeId::HYSPEX_WRID_UNDEFINED;
    std::size_t mSpatialSize = 0;
    std::size_t mSpectralSize = 0;

    // This will be identical to SpatialSize/SpectralSize if no Spatial ROI is in effect.
    std::size_t mMaxSpatialSize = 0;
    std::size_t mMaxSpectralSize = 0;

    // Max pixel value, 2 ^ bpp - 1 (bits per pixel).
    unsigned short mMaxPixelValue = 0;

    cHySpexVNIR_3000N_Serializer mSerializer;
};

