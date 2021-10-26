
#include "SpidercamModel.hpp"
#include "../Utilities/Constants.hpp"

#include <QMessageBox>


cSpidercamModel::cSpidercamModel()
{
    mPositionTolerance_mm = 0.0;
}

cSpidercamModel::~cSpidercamModel()
{

}

void cSpidercamModel::configure(const nlohmann::json& jsonCfg)
{
    std::string c2_ip;
    std::string c3_ip;
    uint16_t port = 0;

    double minX_mm = 0;
    double maxX_mm = 0;
    double minY_mm = 0;
    double maxY_mm = 0;
    double minHeight_mm = 0;
    double maxHeight_mm = 0;
    double maxSpeed_mmps = 0;

    try
    {
        c2_ip = static_cast<std::string>(jsonCfg["c2_ip"]);
        c3_ip = static_cast<std::string>(jsonCfg["c3_ip"]);
        port = jsonCfg["port"];

        minX_mm = jsonCfg["min X position (m)"] * nConstants::M_TO_MM;
        maxX_mm = jsonCfg["max X position (m)"] * nConstants::M_TO_MM;
        minY_mm = jsonCfg["min Y position (m)"] * nConstants::M_TO_MM;
        maxY_mm = jsonCfg["max Y position (m)"] * nConstants::M_TO_MM;
        minHeight_mm = jsonCfg["min height (m)"] * nConstants::M_TO_MM;
        maxHeight_mm = jsonCfg["max height (m)"] * nConstants::M_TO_MM;
        maxSpeed_mmps = jsonCfg["max speed (m/s)"] * nConstants::M_TO_MM;

        double interval_ms = jsonCfg["update interval (ms)"];
        if ((interval_ms <= 0) || (interval_ms > 60000.0))
        {
            QString str = "Invalid \"update interval (ms)\" in the \"spidercam\" configuration.\n";
            str.append("The interval must be in the range >0 to <60,000.  The value will be ignored.");
            QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
            msg.exec();
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
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }

    spidercam::sWorkingDimensions limits = { static_cast<uint32_t>(minX_mm), static_cast<uint32_t>(maxX_mm), 
        static_cast<uint32_t>(minY_mm), static_cast<uint32_t>(maxY_mm),
        static_cast<uint32_t>(minHeight_mm), static_cast<uint32_t>(maxHeight_mm) };

    mController.setSafetyLimits(limits, maxSpeed_mmps);

    QString msg("Trying to establishing connection to Spidercam at ");
    msg.append(c2_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

/*
    if (!mController.try_to_connect(c2_ip, port))
    {
        QMessageBox msg(QMessageBox::Critical, "Spidercam Error", "Could not establish required command connection to Spidercam C2 computer!");
        msg.exec();

#ifdef NDEBUG
        exit(EXIT_FAILURE);
#endif // NDEBUG
    }
*/

    mController.clearIncomingBuffer();

    if (mController.isConnected())
    {
        mController.requestCurrentPosition();
    }

    mCurrentPosition = mController.getLastKnownPosition();
}

void cSpidercamModel::writeDataHeader(cBlockDataFile& file)
{

}

void cSpidercamModel::run()
{
    if (mController.checkForReply())
    {
        mController.readReply();
    }

    const auto& pos = mController.getLastKnownPosition();

    if (spidercam::hasPositionChanged(pos, mCurrentPosition, mPositionTolerance_mm))
    {
        mCurrentPosition = pos;
        emit positionChanged(mCurrentPosition);
    }

    mInError = mController.isInError();
    mBusy = mController.isBusy();

//    bool readyForMotion = mController.isDollyConnected() && mController.isConsoleConnected() && mController.isSetPointEnabled() && !mController.isBusy() && !mController.isInError();
    if (mTimer.elapsed())
    {
        mController.sendRequestForCurrentPosition();
    }
}

