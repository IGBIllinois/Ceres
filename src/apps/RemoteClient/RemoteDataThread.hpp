
#pragma once

#include "DataThread.hpp"

// Forward Declarations
class cRemoteDataModel;
class cSensorController;


class cRemoteDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cRemoteDataThread(cRemoteDataModel* pController);
    virtual ~cRemoteDataThread();

    bool startCommunications() override;
    bool stopCommunications() override;

public:
    cRemoteDataModel* mpController;
};

