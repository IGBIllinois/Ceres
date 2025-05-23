
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
    /*
     * Returns a string used as a descriptor of the sensor.
     */
    virtual const char* descriptor() const = 0;
    virtual uint32_t version() const = 0;
    virtual const std::string& manufacturer() const = 0;
    virtual const std::string& model() const = 0;
    virtual const std::string& serial_number() const = 0;

    virtual const std::string& name() const = 0;
    virtual const std::string& instance() const = 0;
    virtual bool has_instance() const = 0;

public:
    bool startTcpServer(const std::string& ip);

    std::string serverIpAddress() const;
    uint16_t serverPort() const;

    bool hasClient() const;

    void update();

protected:
    /**
     * @brief processStream
     * The processStream method convert the byte stream from a TCP
     * receiver port.
     */
    virtual void processStream(const void* pBuffer, std::size_t buf_length) = 0;

    /**
     * @brief sendOutgoingData
     * Sends any outgoing to the property page.
     */
    virtual int sendOutgoingData(const char* data, std::size_t len);

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