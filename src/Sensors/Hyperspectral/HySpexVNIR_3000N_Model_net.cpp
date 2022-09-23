
#include "HySpexVNIR_3000N_Model_net.hpp"
#include "../../Utilities/Constants.hpp"

#include <optional>

cHySpexVNIR_3000N_Model_net::cHySpexVNIR_3000N_Model_net(QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent)
{
    mConnected = false;
}

bool cHySpexVNIR_3000N_Model_net::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        cHySpexVNIR_3000N_Model::configure(jsonCfg);
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

bool cHySpexVNIR_3000N_Model_net::startCommunications()
{
    mConnected = true;

    return true;
}

void cHySpexVNIR_3000N_Model_net::stopCommunications()
{

    mConnected = false;
}

void cHySpexVNIR_3000N_Model_net::update()
{
    if (!mConnected) return;
}

void cHySpexVNIR_3000N_Model_net::writeDataHeader()
{
}




