
#include "TeledyneDiscoverCameras.hpp"

#include "teledyne_atlas_connect/TeledyneFlirCameraFactory.hpp"

//-----------------------------------------------------------------------------
cFindCameras::cFindCameras(cTeledyneFlirCameraFactory* pFactory, const std::string& interface_name, long long timeout_sec)
    : mpFactory(pFactory)
{
    mInterfaceName = interface_name;
    mTimeout_sec = timeout_sec;
}

cFindCameras::~cFindCameras()
{}

void cFindCameras::searchForCameras()
{
    mpFactory->discoverCameras(mInterfaceName, mTimeout_sec);

    int num_of_cameras = mpFactory->numOfCameras();

    emit camerasFound(num_of_cameras);
}

//-----------------------------------------------------------------------------
cDiscoverThread::cDiscoverThread(cTeledyneFlirCameraFactory* pFactory, const std::string& interface_name, long long timeout_sec)
    : mpFactory(pFactory)
{
    cFindCameras* pFindCameras = new cFindCameras(pFactory, interface_name, timeout_sec);

    pFindCameras->moveToThread(&searchThread);

    connect(&searchThread, &QThread::finished, pFindCameras, &QObject::deleteLater);
    connect(this, &cDiscoverThread::startSearchForCameras, pFindCameras, &cFindCameras::searchForCameras);
    connect(pFindCameras, &cFindCameras::camerasFound, this, &cDiscoverThread::onCamerasFound);
    searchThread.start();
}

cDiscoverThread::~cDiscoverThread()
{
    searchThread.quit();
    searchThread.wait();
}

bool cDiscoverThread::isSearching() const
{
    return mSearching;
}

void cDiscoverThread::startSearch()
{
    if (searchThread.isFinished() || mSearching)
        return;

    emit startSearchForCameras();

    mSearching = true;
}

void cDiscoverThread::stopSearch()
{
    if (searchThread.isFinished())
        return;

    searchThread.quit();
}

void cDiscoverThread::onCamerasFound(int num_of_cameras)
{
    mSearching = false;
    mSearchComplete = true;

    mNumOfCameras = num_of_cameras;

    emit discoverComplete();
}

