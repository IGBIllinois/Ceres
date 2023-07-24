
#pragma once

#include "TimestampProvider.hpp"

#include <QObject>
//#include <nlohmann/json.hpp>

// Forward Declarations
class cSensorModel;

/*****************************************************************************
 * 
 * The cDataModel class is the base class for data acquisition.
 * 
 *****************************************************************************/
class cDataModel : public QObject, public cTimestampProvider
{
    Q_OBJECT

public:
    explicit cDataModel(QObject* parent = nullptr);
    ~cDataModel();

    virtual std::size_t sensorCount() const = 0;
    virtual void addSensor(cSensorModel* pSensor) = 0;

signals:
    void statusMessage(QString msg) const;
    void infoMessage(QString title, QString msg) const;
    void warningMessage(QString title, QString msg) const;
    void errorMessage(QString title, QString msg) const;

public slots:
    virtual void startDataThread() = 0;
    virtual void stopDataThread() = 0;

protected slots:
    void onStatusUpdate(QString msg);
    void onErrorUpdate(QString title, QString msg);
};

