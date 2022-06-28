
#pragma once

#include "DataThread.hpp"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <nlohmann/json.hpp>
#include <chrono>

// Forward Declarations
class cSensorModel;

namespace experiment
{
    enum class State : uint8_t;
}

/*****************************************************************************
 * 
 * The cDataModel class is the base class for data acquisition.
 * 
 *****************************************************************************/
class cDataModel : public QObject
{
    Q_OBJECT

public:
    explicit cDataModel(QObject* parent = nullptr);
    ~cDataModel();

    static std::uint64_t timestamp_ns();

    void addSensor(cSensorModel* pSensor);

    virtual void startDataThread();
    virtual void stopDataThread();

    virtual bool openDataFile(const QString& defaultPath) = 0;
    virtual void closeDataFile() = 0;

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

private slots:
    void onStatusUpdate(QString msg);
    void onErrorUpdate(QString title, QString msg);

protected:
    cDataThread mThread;

private:
    static std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
};

