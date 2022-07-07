
#pragma once

#include "DataThread.hpp"


class cRemoteDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cRemoteDataThread();
    virtual ~cRemoteDataThread();

    bool startCommunications() override;
    bool stopCommunications() override;
};

