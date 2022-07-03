
#pragma once

#include "TimestampProvider.hpp"
#include "DataThread.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
#include <list>

#include <QByteArray>
#include <QtNetwork/QTcpServer>


// Forward Declarations
class cSensorModel;

/*****************************************************************************
 * 
 * The cRemoteDataModel class is the base class for data acquisition.
 * 
 *****************************************************************************/
class cRemoteDataModel : public QObject, public cTimestampProvider
{
    Q_OBJECT

public:
    explicit cRemoteDataModel(QObject* parent = nullptr);
    ~cRemoteDataModel();

    void addSensor(cSensorModel* pSensor);

    virtual void startDataThread();
    virtual void stopDataThread();

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

    QTcpServer* mpTcpServer;

    std::list<QTcpSocket*> mConnections;
};


