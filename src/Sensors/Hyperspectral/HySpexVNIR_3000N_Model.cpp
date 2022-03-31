
#include "HySpexVNIR_3000N_Model.hpp"
#include "HySpexFactory.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cHySpexVNIR_3000N_Model::cHySpexVNIR_3000N_Model(QObject* parent)
:
    cHyperspectralModel(parent)
{
    mConnected = false;
}

char* cHySpexVNIR_3000N_Model::descriptor() const
{
    return hyspex_id;
}

bool cHySpexVNIR_3000N_Model::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        cHyperspectralModel::configure(jsonCfg);
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"hyspex\" configuration: ";
        str.append(e.what());
        emit errorMessage("Configuration Error", str);
        return false;
    }

    return true;
}

bool cHySpexVNIR_3000N_Model::startCommunications()
{
    mConnected = true;

    return true;
}

void cHySpexVNIR_3000N_Model::stopCommunications()
{

    mConnected = false;
}

void cHySpexVNIR_3000N_Model::update()
{
    if (!mConnected) return;
}

void cHySpexVNIR_3000N_Model::writeDataHeader(cBlockDataFile& file)
{
    mSerializer.attach(&file);
}

void cHySpexVNIR_3000N_Model::endDataRecording()
{
    cHyperspectralModel::endDataRecording();
    mSerializer.detach();
}


