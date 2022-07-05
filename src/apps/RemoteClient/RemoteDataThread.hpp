
#pragma once

#include "DataThread.hpp"


class cRemoteDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cRemoteDataThread();
    virtual ~cRemoteDataThread();
};

