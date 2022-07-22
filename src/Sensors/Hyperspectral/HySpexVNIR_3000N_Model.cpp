
#include "HySpexVNIR_3000N_Model.hpp"
#include "HySpexFactory.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cHySpexVNIR_3000N_Model::cHySpexVNIR_3000N_Model(QObject* parent)
:
    cHyperspectralModel("VNIR 3000N", parent)
{
    mConnected = false;
}

const char* cHySpexVNIR_3000N_Model::descriptor() const
{
    return hyspex_id;
}

uint16_t cHySpexVNIR_3000N_Model::data_class_id() const
{
    return mSerializer.classID();
}

bool cHySpexVNIR_3000N_Model::configure(const nlohmann::json& jsonCfg)
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




