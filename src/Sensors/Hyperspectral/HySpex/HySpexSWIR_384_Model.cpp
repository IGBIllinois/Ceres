
#include "HySpexSWIR_384_Model.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <optional>

cHySpexSWIR_384_Model::cHySpexSWIR_384_Model(QObject* parent)
:
    cHyperspectralModel("SWIR 384", parent)
{
    mManufacturer = "HySpex";
    mModel = "SWIR 384";
}

const char* cHySpexSWIR_384_Model::descriptor() const
{
    return hyspex_id;
}

uint16_t cHySpexSWIR_384_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexSWIR_384_Model::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        cHyperspectralModel::configure(jsonCfg);

    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex\" configuration: ";
        msg.append(e.what());
        emit logMessage(logERROR, q_name(), msg);
        return false;
    }

    return true;
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
}


