
#include "HySpexVNIR_3000N_Model.hpp"
#include "Constants.hpp"

#include <optional>
#include <iostream>


namespace
{
    static uint8_t vnir_3000N_device_id = 0;
}


cHySpexVNIR_3000N_Model::cHySpexVNIR_3000N_Model(QObject* parent)
:
    cHySpexCameraModel("VNIR-3000N", parent), mDeviceID(++vnir_3000N_device_id)
{
    mModel = "VNIR-3000N";
}

cHySpexVNIR_3000N_Model::~cHySpexVNIR_3000N_Model()
{}

uint8_t cHySpexVNIR_3000N_Model::device_id() const
{
    return mDeviceID;
}

uint16_t cHySpexVNIR_3000N_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexVNIR_3000N_Model::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        return cHySpexCameraModel::configure(jsonCfg);
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex\" configuration: ";
        msg.append(e.what());
        logMessage(logERROR, msg);
    }

    return false;
}

void cHySpexVNIR_3000N_Model::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cHySpexVNIR_3000N_Model::disableDataRecording()
{
    cHyperspectralModel::disableDataRecording();
    mSerializer.detach();
}

void cHySpexVNIR_3000N_Model::writeDataHeader()
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
    mSerializer.writeSensorTemperature_C(mDeviceID, mSensorTemp_C);

    mSerializer.writeLensName(mDeviceID, mLens);
    mSerializer.writeLensWorkingDistance_cm(mDeviceID, mWorkingDistance_cm);
    mSerializer.writeLensFieldOfView_deg(mDeviceID, mFieldOfView_deg);

    mSerializer.writeNumOfBackgrounds(mDeviceID, mNumBackgrounds);

    mSerializer.writeResponsivityMatrix(mDeviceID, mResponsivityMatrix);
    mSerializer.writeQuantumEfficiencyData(mDeviceID, mQuantumEfficiencyData);
    mSerializer.writeSpectralCalibration(mDeviceID, mSpectralCalibrationPerBand);
    mSerializer.writeBadPixelCorrection(mDeviceID, mBadPixelCorrectionData);
}

void cHySpexVNIR_3000N_Model::onStartingReferenceMeasurement()
{
//    if (mIsRecording && mSerializer)
    if (mSerializer)
    {
        QString msg = "VNIR 3000N received the start of reference measurement.";
        logMessage(logINFO, msg);

        mSerializer.writeBeginOfReference(mDeviceID);
    }
}

void cHySpexVNIR_3000N_Model::onEndingReferenceMeasurement()
{
//    if (mIsRecording && mSerializer)
    if (mSerializer)
    {
        QString msg = "VNIR 3000N received the end of reference measurement.";
        logMessage(logINFO, msg);

        mSerializer.writeEndOfReference(mDeviceID);
    }
}


