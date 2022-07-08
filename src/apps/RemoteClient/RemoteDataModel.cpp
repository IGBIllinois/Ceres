
#include "RemoteDataModel.hpp"

#include "SensorModel.hpp"

#include <QTcpServer>
#include <QString>

cRemoteDataModel::cRemoteDataModel(QObject* parent)
    :
    cDataModel(parent),
    mpTcpServer(nullptr)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cRemoteDataModel::onStatusUpdate);

    mpTcpServer = new QTcpServer();

    QObject::connect(mpTcpServer, &QTcpServer::acceptError, this, &cRemoteDataModel::acceptError);
    QObject::connect(mpTcpServer, &QTcpServer::newConnection, this, &cRemoteDataModel::newConnection);
}

cRemoteDataModel::~cRemoteDataModel()
{
    mpTcpServer->close();
}

bool cRemoteDataModel::startTcpServer(const std::string& ip, uint16_t port)
{
    QHostAddress local_endpoint(ip.c_str());

    return mpTcpServer->listen(local_endpoint, port);
}

void cRemoteDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        if (!pSensor->initialize())
        {
            emit statusMessage("Sensor failed initialization!");
        }

        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

void cRemoteDataModel::startDataThread()
{
    mThread.start();
}

void cRemoteDataModel::stopDataThread()
{
    mThread.stop();
}

bool cRemoteDataModel::openDataFile(const QString& fileName)
{
    if (fileName.isEmpty())
        return false;

    if (mFile.isOpen())
        return false;

    QString qualifiedFileName = fileName;

    auto ext = qualifiedFileName.lastIndexOf('.');

    char timestamp[100];

    std::time_t t = std::time(nullptr);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&t));

    qualifiedFileName.insert(ext, "_");
    qualifiedFileName.insert(ext + 1, timestamp);

    std::replace_if(qualifiedFileName.begin(), qualifiedFileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    mFile.open(qualifiedFileName.toStdString());

    mSerializer.attach(&mFile);

    return mFile.isOpen();

}

void cRemoteDataModel::closeDataFile()
{
    mSerializer.endTime(time(nullptr));
    mSerializer.detach();
    mFile.close();
}

void cRemoteDataModel::acceptError(QAbstractSocket::SocketError socketError)
{

}

void cRemoteDataModel::newConnection()
{
    QTcpSocket* client = mpTcpServer->nextPendingConnection();

    if (client)
    {
        emit statusMessage("Connected to client.");
        mConnections.push_back(client);
    }
}

