
#pragma once

#include "DataFile.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>

// Forward Declarations

class cSensorModel;

class cDataModel : public QThread
{
    Q_OBJECT

public:
    explicit cDataModel(QObject* parent = nullptr);
    ~cDataModel();

    void addSensor(cSensorModel* pSensor);

    void startDataCollection();
    void stopDataCollection();

    void startDataRecording(const std::string& filename);
    void stopDataRecording();

signals:
    void statusMessage(QString msg);

private slots:
    void onStatusUpdate(QString msg);

protected:
    void run() override;

private:
    std::vector<cSensorModel*> mActiveSensors;

    cDataFile   mFile;

    QMutex mMutex;
    bool mAbort = false;

};

