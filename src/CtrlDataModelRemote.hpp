
#pragma once

#include "CtrlDataModel.hpp"
#include "BlockDataFile/BlockDataFile.hpp"
#include "BlockDataFile/ExperimentSerializer.hpp"

#include <QByteArray>
#include <QtNetwork/QTcpSocket>

class cCtrlDataModelRemote : public cCtrlDataModel
{
    Q_OBJECT

public:
    explicit cCtrlDataModelRemote(QObject* parent = nullptr);
    ~cCtrlDataModelRemote();

    bool try_to_connect(const QString& hostname, uint16_t port, 
                        bool use_ipv6, const QString& local_ip);

    bool openDataFile(const QString& defaultPath) override;
    void closeDataFile();

    void startExperiment() override;

private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);

private:
//    cBlockDataFileWriter    mFile;
//    cExperimentSerializer   mSerializer;

    bool mConnected;

    QTcpSocket mSocket;
    QByteArray mReplyBuffer;

};

