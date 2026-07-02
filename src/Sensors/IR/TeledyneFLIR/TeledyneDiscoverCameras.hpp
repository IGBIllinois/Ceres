
#pragma once

#include <QObject>
#include <QThread>

#include <string>

// Forward Declarations
class cTeledyneFlirCameraFactory;


class cFindCameras : public QObject
{
	Q_OBJECT

public:
	// ctor(s)
	cFindCameras(cTeledyneFlirCameraFactory* pFactory, const std::string& interface_name, long long timeout_sec);
	virtual ~cFindCameras();

signals:
	void camerasFound(int num_of_cameras);

public slots:
	void searchForCameras();

private:
	cTeledyneFlirCameraFactory* mpFactory;

	std::string mInterfaceName = "usb";
	long long mTimeout_sec = 5;
};


// Define a new frame type: this is going to be our main frame
class cDiscoverThread : public QObject
{
	Q_OBJECT

	QThread searchThread;

public:
	cDiscoverThread(cTeledyneFlirCameraFactory* pFactory, const std::string& interface_name, long long timeout_sec);
	virtual ~cDiscoverThread();

	bool isSearching() const;

	void startSearch();
	void stopSearch();

signals:
	void startSearchForCameras();
	void discoverComplete();

public slots:
	void onCamerasFound(int num_of_cameras);

private:
	bool mSearching = false;
	bool mSearchComplete = false;
	
	int mNumOfCameras = 0;

    cTeledyneFlirCameraFactory* mpFactory;
};
