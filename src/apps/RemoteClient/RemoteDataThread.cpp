
#include "RemoteDataThread.hpp"

cRemoteDataThread::cRemoteDataThread()
{
}

cRemoteDataThread::~cRemoteDataThread()
{
}


bool cRemoteDataThread::startCommunications()
{
    QString msg("Starting communications...");
    emit statusMessage(msg);

    if (!cDataThread::startCommunications())
    {
        return false;
    }

    return true;
}

bool cRemoteDataThread::stopCommunications()
{
    QString msg("Stopping communications...");
    emit statusMessage(msg);

    cDataThread::stopCommunications();

    return true;
}


