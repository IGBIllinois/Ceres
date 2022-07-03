
#pragma once

#include <QThread>
#include <vector>

// Forward Declarations
class cSensorModel;

class cDataThread : public QThread
{
    Q_OBJECT

public:
    explicit cDataThread();
    ~cDataThread();

    void start();
    void stop();

signals:
    void statusMessage(QString msg);

protected:
    void run() override;
    virtual void updateAll();

    virtual bool startCommunications();
    virtual bool stopCommunications();

public:
    std::vector<cSensorModel*> mSensors;
    std::vector<cSensorModel*> mActiveSensors;

private:
    bool mAbort = false;
};

