
#include "HySpexSWIR_384_Model.hpp"
#include "Constants.hpp"

#include <optional>


namespace
{
    static uint8_t swir_384_device_id = 0;
}


cHySpexSWIR_384_Model::cHySpexSWIR_384_Model(QObject* parent)
:
    cHySpexCameraModel("SWIR-384", parent), mDeviceID(++swir_384_device_id)
{
    mModel = "SWIR-384";
}

uint8_t cHySpexSWIR_384_Model::device_id() const
{
    return mDeviceID;
}

uint16_t cHySpexSWIR_384_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexSWIR_384_Model::configure(const nlohmann::json& jsonCfg)
{
    try
    {
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex\" configuration: ";
        msg.append(e.what());
        emit logMessage(logERROR, q_name(), msg);
        return false;
    }

    return cHySpexCameraModel::configure(jsonCfg);
}

void cHySpexSWIR_384_Model::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cHySpexSWIR_384_Model::disableDataRecording()
{
    cHyperspectralModel::disableDataRecording();
    mSerializer.detach();
}

void cHySpexSWIR_384_Model::writeDataHeader()
{
    mSerializer.writeID(mDeviceID, mID);
    mSerializer.writeSerialNumber(mDeviceID, mSerialNumber);
    switch (mWavelengthRangeId)
    {
    case hyspex::WavelengthRangeId::HYSPEX_WRID_SWIR:
        mSerializer.writeWavelengthRange_nm(mDeviceID, 1000, 2500);
        break;
    case hyspex::WavelengthRangeId::HYSPEX_WRID_SWIRi:
        mSerializer.writeWavelengthRange_nm(mDeviceID, 1000, 1700);
        break;
    case hyspex::WavelengthRangeId::HYSPEX_WRID_VNIR:
        mSerializer.writeWavelengthRange_nm(mDeviceID, 400, 1000);
        break;
    }

    mSerializer.writeSpatialSize(mDeviceID, mSpatialSize);
    mSerializer.writeSpectralSize(mDeviceID, mSpectralSize);

    mSerializer.writeMaxSpatialSize(mDeviceID, mMaxSpatialSize);
    mSerializer.writeMaxSpectralSize(mDeviceID, mMaxSpectralSize);

    mSerializer.writeMaxPixelValue(mDeviceID, mMaxPixelValue);

    mSerializer.writeAverageFrames(mDeviceID, mAverageFrames);
    mSerializer.writeFramePeriod_us(mDeviceID, mFramePeriod_us);
    mSerializer.writeIntegrationTime_us(mDeviceID, mIntegrationTime_us);
    mSerializer.writeAmbientTemperature_C(mDeviceID, mAmbientTemp_C);

    double temp_K = 273.15 + mSensorTemp_C;
    mSerializer.writeSensorTemperature_K(mDeviceID, temp_K);

    mSerializer.writeLensName(mDeviceID, mLens);
    mSerializer.writeLensWorkingDistance_cm(mDeviceID, mWorkingDistance_cm);
    mSerializer.writeLensFieldOfView_deg(mDeviceID, mFieldOfView_deg);

    mSerializer.writeNumOfBackgrounds(mDeviceID, mNumBackgrounds);

    mSerializer.writeResponsivityMatrix(mDeviceID, mResponsivityMatrix);
    mSerializer.writeQuantumEfficiencyData(mDeviceID, mQuantumEfficiencyData);
    mSerializer.writeSpectralCalibration(mDeviceID, mSpectralCalibrationPerBand);
    mSerializer.writeBadPixelCorrection(mDeviceID, mBadPixelCorrectionData);
}




