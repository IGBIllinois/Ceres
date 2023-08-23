
#include "HySpexVNIR_3000N_Model.hpp"
#include "Constants.hpp"

#include <optional>
#include <iostream>


cHySpexVNIR_3000N_Model::cHySpexVNIR_3000N_Model(QObject* parent)
:
    cHySpexCameraModel("VNIR-3000N", parent)
{
    mModel = "VNIR-3000N";
}

cHySpexVNIR_3000N_Model::~cHySpexVNIR_3000N_Model()
{}

uint16_t cHySpexVNIR_3000N_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexVNIR_3000N_Model::configure(const nlohmann::json& jsonCfg)
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
    mSerializer.writeID(mID);
    mSerializer.writeSerialNumber(mSerialNumber);
    switch (mWavelengthRangeId)
    {
    case hyspex::WavelengthRangeId::HYSPEX_WRID_SWIR:
        mSerializer.writeWavelengthRange_nm(1000, 2500);
        break;
    case hyspex::WavelengthRangeId::HYSPEX_WRID_SWIRi:
        mSerializer.writeWavelengthRange_nm(1000, 1700);
        break;
    case hyspex::WavelengthRangeId::HYSPEX_WRID_VNIR:
        mSerializer.writeWavelengthRange_nm(400, 1000);
        break;
    }

    mSerializer.writeSpatialSize(mSpatialSize);
    mSerializer.writeSpectralSize(mSpectralSize);

    mSerializer.writeMaxSpatialSize(mMaxSpatialSize);
    mSerializer.writeMaxSpectralSize(mMaxSpectralSize);

    mSerializer.writeMaxPixelValue(mMaxPixelValue);

    mSerializer.writeAverageFrames(mAverageFrames);
    mSerializer.writeFramePeriod_us(mFramePeriod_us);
    mSerializer.writeIntegrationTime_us(mIntegrationTime_us);
    mSerializer.writeAmbientTemperature_C(mAmbientTemp_C);
    mSerializer.writeSensorTemperature_C(mSensorTemp_C);

    mSerializer.writeLensName(mLens);
    mSerializer.writeLensWorkingDistance_cm(mWorkingDistance_cm);
    mSerializer.writeLensFieldOfView_deg(mFieldOfView_deg);

    mSerializer.writeNumOfBackgrounds(mNumBackgrounds);

    mSerializer.writeResponsivityMatrix(mResponsivityMatrix);
    mSerializer.writeQuantumEfficiencyData(mQuantumEfficiencyData);
    mSerializer.writeSpectralCalibration(mSpectralCalibrationPerBand);
    mSerializer.writeBadPixelCorrection(mBadPixelCorrectionData);
}



