
#include "HySpexVNIR_3000N_Model_net.hpp"
#include "Constants.hpp"

#include <optional>

cHySpexVNIR_3000N_Model_net::cHySpexVNIR_3000N_Model_net(QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent)
{
    mConnected = false;
}

void cHySpexVNIR_3000N_Model_net::updateViews()
{

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

void cHySpexVNIR_3000N_Model_net::setAverageFrames(std::uint16_t frames)
{}

void cHySpexVNIR_3000N_Model_net::setFramePeriod_us(std::uint32_t frame_period_us)
{}

void cHySpexVNIR_3000N_Model_net::setIntegrationTime_us(std::uint32_t integration_time_us)
{}

void cHySpexVNIR_3000N_Model_net::setNumOfBackgrounds(int num_backgrounds)
{
    if (num_backgrounds < 0) num_backgrounds = 0;
    if (num_backgrounds > 1000) num_backgrounds = 1000;
    mNumBackgrounds = num_backgrounds;
}

void cHySpexVNIR_3000N_Model_net::calcBackground()
{

}




