
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

    virtual void addSensor(cSensorModel* pSensor) = 0;

    virtual void startDataThread() = 0;
    virtual void stopDataThread() = 0;

    virtual bool openDataFile(const QString& defaultPath) = 0;
    virtual void closeDataFile() = 0;

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

protected slots:
    void onStatusUpdate(QString msg);
    void onErrorUpdate(QString title, QString msg);
};

