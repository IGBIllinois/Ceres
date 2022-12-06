
#include "HySpexSWIR_384_Model_net.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <optional>

cHySpexSWIR_384_Model_net::cHySpexSWIR_384_Model_net(QObject* parent)
:
    cHySpexSWIR_384_Model(parent)
{
    mConnected = false;
}

bool cHySpexSWIR_384_Model_net::configure(const nlohmann::json& jsonCfg)
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

bool cHySpexSWIR_384_Model_net::startCommunications()
{
    mConnected = true;

    return true;
}

void cHySpexSWIR_384_Model_net::stopCommunications()
{
    mConnected = false;
}

void cHySpexSWIR_384_Model_net::update()
{
    if (!mConnected) return;

}

void cHySpexSWIR_384_Model_net::writeDataHeader()
{
}


