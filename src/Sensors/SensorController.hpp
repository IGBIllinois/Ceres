
#pragma once

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpServer>

class cSensorController : public QObject
{
	Q_OBJECT

public:
	cSensorController(QObject* parent = nullptr);
	virtual ~cSensorController();

public:
    bool startTcpServer(const std::string& ip);

    uint16_t serverPort() const;

    bool hasClient() const;

protected:
    /**
     * @brief processStream
     * The processStream method convert the byte stream from a TCP
     * receiver port.
     */
    virtual void processStream(const void* pBuffer, std::size_t buf_length) = 0;

public slots:

/*
 * Signals handlers from the TCP server
 */
private slots:
    void acceptError(QAbstractSocket::SocketError socketError);
    void newConnection();

/*
 * Signals handlers from the client TCP socket
 */
private slots:
    void processNewCommand();
    void clientDisconnected();
    void clientErrorOccurred(QAbstractSocket::SocketError socketError);
    void clientStateChanged(QAbstractSocket::SocketState socketState);

protected:
	QTcpServer* mpTcpServer = nullptr;
	QTcpSocket* mpClient = nullptr;

private:
};