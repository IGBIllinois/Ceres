
#include "HySpexSWIR_384_Model.hpp"
#include "HySpexFactory.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cHySpexSWIR_384_Model::cHySpexSWIR_384_Model(QObject* parent)
:
    cHyperspectralModel(parent)
{
    mConnected = false;
}

char* cHySpexSWIR_384_Model::descriptor() const
{
    return hyspex_id;
}

bool cHySpexSWIR_384_Model::configure(const nlohmann::json& jsonCfg)
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

bool cHySpexSWIR_384_Model::startCommunications()
{
    mConnected = true;

    return true;
}

void cHySpexSWIR_384_Model::stopCommunications()
{
    mConnected = false;
}

void cHySpexSWIR_384_Model::update()
{
    if (!mConnected) return;

}

void cHySpexSWIR_384_Model::writeDataHeader(cBlockDataFile& file)
{
    mSerializer.attach(&file);
}

void cHySpexSWIR_384_Model::endDataRecording()
{
    cHyperspectralModel::endDataRecording();
    mSerializer.detach();
}


