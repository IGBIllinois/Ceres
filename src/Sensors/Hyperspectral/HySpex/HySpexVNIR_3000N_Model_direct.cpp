
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "Constants.hpp"

#include <optional>

cHySpexVNIR_3000N_Model_direct::cHySpexVNIR_3000N_Model_direct(QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent)
{
}

bool cHySpexVNIR_3000N_Model_direct::configure(const nlohmann::json& jsonCfg)
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

bool cHySpexVNIR_3000N_Model_direct::startCommunications()
{
    return true;
}

void cHySpexVNIR_3000N_Model_direct::stopCommunications()
{
}

void cHySpexVNIR_3000N_Model_direct::update()
{
}

void cHySpexVNIR_3000N_Model_direct::writeDataHeader()
{
}




