
#pragma once

#include "DataModel.hpp"
#include "RemoteDataThread.hpp"
#include "BlockDataFile.hpp"
#include "ExperimentSerializer.hpp"

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
class cRemoteDataModel : public cDataModel
{
    Q_OBJECT

public:
    explicit cRemoteDataModel(QObject* parent = nullptr);
    virtual ~cRemoteDataModel();

    bool startTcpServer(const std::string& ip, uint16_t port);

    void addSensor(cSensorModel* pSensor) override;

    void startDataThread() override;
    void stopDataThread() override;

    bool openDataFile(const QString& fileName) override;
    void closeDataFile() override;

private slots:
    void acceptError(QAbstractSocket::SocketError socketError);
    void newConnection();

protected:
    cBlockDataFileWriter    mFile;
    cExperimentSerializer   mSerializer;

    cRemoteDataThread mThread;

    QTcpServer* mpTcpServer;

    std::list<QTcpSocket*> mConnections;
};


