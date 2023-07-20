
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
}




