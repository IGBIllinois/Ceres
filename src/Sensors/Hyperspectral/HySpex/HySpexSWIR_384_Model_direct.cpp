
#include "HySpexSWIR_384_Model_direct.hpp"
#include "HySpexFactory.hpp"
#include "Constants.hpp"

#include <HySpexConnect/SWIR384.hpp>

#include <optional>

cHySpexSWIR_384_Model_direct::cHySpexSWIR_384_Model_direct(std::unique_ptr<hyspex::cSWIR384> camera, QObject* parent)
:
    cHySpexSWIR_384_Model(parent), mCamera(std::move(camera))
{
}

cHySpexSWIR_384_Model_direct::~cHySpexSWIR_384_Model_direct() {}

bool cHySpexSWIR_384_Model_direct::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        cHySpexSWIR_384_Model::configure(jsonCfg);

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

bool cHySpexSWIR_384_Model_direct::startCommunications()
{
    return true;
}

void cHySpexSWIR_384_Model_direct::stopCommunications()
{
}

void cHySpexSWIR_384_Model_direct::update()
{
}

void cHySpexSWIR_384_Model_direct::writeDataHeader()
{
}


