
#include "OusterAsyncCommands.hpp"

#include "OusterModel_net.hpp"
#include "../../Utilities/Constants.hpp"

#include <ouster/ouster_utils.h>
#include <optional>
#include <string>


///////////////////////////////////////////////////////////////////////////////
// The OUSTER Async Command classes
///////////////////////////////////////////////////////////////////////////////

cOusterAsyncCmd::cOusterAsyncCmd(cOusterModel_net* pModel) : mpModel(pModel) {};

cOusterCmdStream_Qt& cOusterAsyncCmd::cmdStream()
{
    return mpModel->mCmdStream;
}

void cOusterAsyncCmd::setStatus(const sensor::eStatus status)
{
    mpModel->setStatus(status);
}

void cOusterAsyncCmd::statusMessage(QString& msg)
{
    mpModel->emitStatusMessage(msg);
}


/// Reinitialization
cOusterAsyncCmd_Reinitialize::cOusterAsyncCmd_Reinitialize(cOusterModel_net* pModel)
    : cOusterAsyncCmd(pModel)
{}

bool cOusterAsyncCmd_Reinitialize::postCmd()
{
    if (!cmdStream().postReinitialize())
    {
        setStatus(sensor::eStatus::FAILED);
        QString msg = "OUSTER failed to reinitialize.";
        statusMessage(msg);
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_Reinitialize::complete()
{
    return cmdStream().reinitializeComplete();
}


// Lidar Mode
cOusterAsyncCmd_SetLidarMode::cOusterAsyncCmd_SetLidarMode(cOusterModel_net* pModel, ouster::eLIDAR_MODE mode)
  : cOusterAsyncCmd(pModel), mMode(mode) 
{}

bool cOusterAsyncCmd_SetLidarMode::postCmd()
{
    setStatus(sensor::eStatus::REINITIALIZING);

    QString msg = "Setting lidar mode to ";
    msg += QString::fromStdString(to_string(mMode));
    statusMessage(msg);

    if (!cmdStream().postLidarMode(mMode))
    {
        setStatus(sensor::eStatus::FAILED);

        QString msg = "Failed to set the lidar mode to ";
        msg += QString::fromStdString(to_string(mMode));
        statusMessage(msg);
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_SetLidarMode::complete()
{
    try
    {
        std::string reply;
        return cmdStream().postSetConfigParamComplete(reply);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception Set Lidar Mode:" << e.what();
        setStatus(sensor::eStatus::FAILED);
    }

    return true;
}


cOusterAsyncCmd_GetLidarMode::cOusterAsyncCmd_GetLidarMode(cOusterModel_net* pModel, ouster::eLIDAR_MODE mode)
    : cOusterAsyncCmd(pModel), mMode(mode)
{}

bool cOusterAsyncCmd_GetLidarMode::postCmd()
{
    if (!cmdStream().queryLidarMode(true))
    {
        setStatus(sensor::eStatus::FAILED);
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_GetLidarMode::complete()
{
    try
    {
        ouster::eLIDAR_MODE mode;
        if (!cmdStream().lidarModeComplete(mode))
        {
            return false;
        }

        bool result = (mode == mMode);

        setStatus(result ? sensor::eStatus::RUNNING : sensor::eStatus::FAILED);

        QString msg = "Lidar mode set to ";
        msg += QString::fromStdString(to_string(mode));
        statusMessage(msg);

        mpModel->setLidarMode(mode);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception Get Lidar Mode:" << e.what();
        setStatus(sensor::eStatus::FAILED);
    }

    return true;
}

/// Azimuth Window 

cOusterAsyncCmd_SetAzimuthWindow::cOusterAsyncCmd_SetAzimuthWindow(cOusterModel_net* pModel, double min_deg, double max_deg)
    : cOusterAsyncCmd(pModel), mMin_deg(min_deg), mMax_deg(max_deg) 
{}

bool cOusterAsyncCmd_SetAzimuthWindow::postCmd()
{
    setStatus(sensor::eStatus::REINITIALIZING);

    QString msg = "Setting azimuth window set to (";
    msg += QString::number(mMin_deg);
    msg += ", ";
    msg += QString::number(mMax_deg);
    msg += ")";

    statusMessage(msg);

    if (!cmdStream().postAzimuthWindow(mMin_deg, mMax_deg))
    {
        setStatus(sensor::eStatus::FAILED);
        qCritical() << "Set azimuth window failed!";
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_SetAzimuthWindow::complete()
{
    try
    {
        std::string reply;
        return cmdStream().postSetConfigParamComplete(reply);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception Set Azimuth Window:" << e.what();
        setStatus(sensor::eStatus::FAILED);
    }

    return true;
}


cOusterAsyncCmd_GetAzimuthWindow::cOusterAsyncCmd_GetAzimuthWindow(cOusterModel_net* pModel, double min_deg, double max_deg)
    : cOusterAsyncCmd(pModel), mMin_deg(min_deg), mMax_deg(max_deg)
{}

bool cOusterAsyncCmd_GetAzimuthWindow::postCmd()
{
    if (!cmdStream().queryAzimuthWindow(true))
    {
        setStatus(sensor::eStatus::FAILED);
        qCritical() << "Query azimuth window failed!";
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_GetAzimuthWindow::complete()
{
    try
    {
        ouster::azimuth_range_t range;
        if (!cmdStream().azimuthWindowComplete(range.min_deg, range.max_deg))
        {
            return false;
        }

        bool result = (range.min_deg == mMin_deg) && (range.max_deg == mMax_deg);

        setStatus(result ? sensor::eStatus::RUNNING : sensor::eStatus::FAILED);

        QString msg = "Azimuth window is set to (";
        msg += QString::number(range.min_deg);
        msg += ", ";
        msg += QString::number(range.max_deg);
        msg += ")";

        statusMessage(msg);

        mpModel->setAzimuthWindow(range);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception Get Azimuth Window:" << e.what();
        setStatus(sensor::eStatus::FAILED);
    }

    return true;
}


/// Lidar Data Format

cOusterAsyncCmd_GetLidarDataFormat::cOusterAsyncCmd_GetLidarDataFormat(cOusterModel_net* pModel)
    : cOusterAsyncCmd(pModel)
{}

bool cOusterAsyncCmd_GetLidarDataFormat::postCmd()
{
    if (!cmdStream().queryLidarDataFormat())
    {
        setStatus(sensor::eStatus::FAILED);
        qCritical() << "Query lidar data format failed!";
        return false;
    }

    return true;
}

bool cOusterAsyncCmd_GetLidarDataFormat::complete()
{
    try
    {
        auto format = cmdStream().lidarDataFormatComplete();
        if (!format.has_value())
        {
            return false;
        }

        mpModel->setLidarDataFormat(format.value());
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception Get Lidar Data Format:" << e.what();
        setStatus(sensor::eStatus::FAILED);
    }

    return true;
}




