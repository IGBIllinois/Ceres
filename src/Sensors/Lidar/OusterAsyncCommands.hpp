
#pragma once

#include <QString>

class cOusterModel_net;
class cOusterCmdStream_Qt;

namespace sensor
{
    enum class eStatus;
}

namespace ouster
{
    enum class eLIDAR_MODE : uint8_t;
}

///////////////////////////////////////////////////////////////////////////////
// The OUSTER Async Command classes
///////////////////////////////////////////////////////////////////////////////

class cOusterAsyncCmd
{
public:
    cOusterAsyncCmd(cOusterModel_net* pModel);
    virtual ~cOusterAsyncCmd() = default;

    virtual bool postCmd() = 0;
    virtual bool complete() = 0;

protected:
    cOusterCmdStream_Qt& cmdStream();

    void setStatus(const sensor::eStatus status);
    void statusMessage(QString& msg);

protected:
    cOusterModel_net* mpModel;
};

// Reinitialization

class cOusterAsyncCmd_Reinitialize : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_Reinitialize(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};

// Lidar Modes 
class cOusterAsyncCmd_SetLidarMode : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_SetLidarMode(cOusterModel_net* pModel,
        ouster::eLIDAR_MODE mode);

    bool postCmd() override;
    bool complete() override;

private:
    ouster::eLIDAR_MODE mMode;
};

class cOusterAsyncCmd_GetLidarMode : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_GetLidarMode(cOusterModel_net* pModel, 
        ouster::eLIDAR_MODE mode);

    bool postCmd() override;
    bool complete() override;

private:
    ouster::eLIDAR_MODE mMode;
};

// Azimuth Window 

class cOusterAsyncCmd_SetAzimuthWindow : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_SetAzimuthWindow(cOusterModel_net* pModel, double min_deg, double max_deg);

    bool postCmd() override;
    bool complete() override;

private:
    double mMin_deg;
    double mMax_deg;
};

class cOusterAsyncCmd_GetAzimuthWindow : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_GetAzimuthWindow(cOusterModel_net* pModel, double min_deg, double max_deg);

    bool postCmd() override;
    bool complete() override;

private:
    double mMin_deg;
    double mMax_deg;
};

// LiDAR Data Format

class cOusterAsyncCmd_GetLidarDataFormat : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_GetLidarDataFormat(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};

// Check LiDAR Status for RUNNING

class cOusterAsyncCmd_WaitForRunning final : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_WaitForRunning(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;

private:
    bool sendCommand();

private:
    bool mWaitingForReply;
};

// Data Collection

class cOusterAsyncCmd_StopDataCollection : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_StopDataCollection(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};

class cOusterAsyncCmd_StartDataCollection : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_StartDataCollection(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};

class cOusterAsyncCmd_PauseDataCollection : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_PauseDataCollection(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};

class cOusterAsyncCmd_RestoreDataCollection : public cOusterAsyncCmd
{
public:
    cOusterAsyncCmd_RestoreDataCollection(cOusterModel_net* pModel);

    bool postCmd() override;
    bool complete() override;
};
