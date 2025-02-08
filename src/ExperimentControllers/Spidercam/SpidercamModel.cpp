
#include "SpidercamModel.hpp"
#include "../../Utilities/Constants.hpp"
#include "ExperimentTypes.hpp"

namespace
{
    uint32_t TOLERANCE_MM = 10;
}

cSpidercamModel::cSpidercamModel(QObject* parent)
:
    cExperimentControlModel(parent),
    mDollyConnected(false), mConsoleConnected(false), mActivated(false),
    mDollyPositionKnown(false), mConsoleEnabled(false), mInInteractiveMode(false),
    mInScriptMode(false)
{
    mPositionTolerance_mm = TOLERANCE_MM;
}

cSpidercamModel::~cSpidercamModel()
{}

void cSpidercamModel::configure(const nlohmann::json& jsonCfg)
{
    double minX_mm = 0;
    double maxX_mm = 0;
    double minY_mm = 0;
    double maxY_mm = 0;
    double minHeight_mm = 0;
    double maxHeight_mm = 0;

    try
    {
        minX_mm = jsonCfg["min X position (m)"] * nConstants::M_TO_MM;
        maxX_mm = jsonCfg["max X position (m)"] * nConstants::M_TO_MM;
        minY_mm = jsonCfg["min Y position (m)"] * nConstants::M_TO_MM;
        maxY_mm = jsonCfg["max Y position (m)"] * nConstants::M_TO_MM;
        minHeight_mm = jsonCfg["min height (m)"] * nConstants::M_TO_MM;
        maxHeight_mm = jsonCfg["max height (m)"] * nConstants::M_TO_MM;
        mMaxSpeed_mmps = jsonCfg["max speed (m/s)"] * nConstants::M_TO_MM;

        double interval_ms = jsonCfg["update interval (ms)"];
        if ((interval_ms <= 0) || (interval_ms > 60000.0))
        {
            QString str = "Invalid \"update interval (ms)\" in the \"spidercam\" configuration.\n";
            str.append("The interval must be in the range >0 to <60,000.  The value will be ignored.");
            emit errorMessage("Configuration Error", str);
        }
        else
        {
            mTimer.interval_ms(static_cast<uint32_t>(interval_ms));
        }

        mPositionTolerance_mm = jsonCfg["position tolerance (cm)"] * nConstants::CM_TO_MM;
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"spidercam\" configuration: ";
        str.append(e.what());
        emit errorMessage("Configuration Error", str);
        return;
    }

    mLimits = { static_cast<uint32_t>(minX_mm), static_cast<uint32_t>(maxX_mm), 
        static_cast<uint32_t>(minY_mm), static_cast<uint32_t>(maxY_mm),
        static_cast<uint32_t>(minHeight_mm), static_cast<uint32_t>(maxHeight_mm) };
}

void cSpidercamModel::startExperiment()
{
    if (!mInScriptMode && !mRunning)
    {
        QString str = "Make sure the C2 computer is in remote mode.\n";
        str += "Please enter remote mode.";
        emit warningMessage("Message", str);
    }

    cExperimentControlModel::startExperiment();
}


void cSpidercamModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSpidercamModel::disableDataRecording()
{
    mSerializer.detach();
}

void cSpidercamModel::writeDataHeader()
{}

void cSpidercamModel::stopDataRecording()
{}


