
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
    const char* descriptor() const;
    uint32_t version() const;
    const std::string& manufacturer() const;
    const std::string& model() const;
    const std::string& serial_number() const;

    const std::string& name() const;
    const std::string& instance() const;
    bool has_instance() const;

public:
    bool startTcpServer(const std::string& ip);

    std::string serverIpAddress() const;
    uint16_t serverPort() const;

    bool hasClient() const;

    void update();

public slots:
    virtual void onDescriptorUpdate(QString descriptor);
    virtual void onVersionUpdate(int version);
    virtual void onManufacturerUpdate(QString manufacturer);
    virtual void onModelUpdate(QString model);
    virtual void onSerialNumberUpdate(QString serial_number);

    virtual void onNameUpdate(QString name);
    virtual void onInstanceUpdate(QString instance);

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
    std::string mDescriptor;
    uint32_t    mVersion = 1;
    std::string mManufacturer;
    std::string mModel;
    std::string mSerialNumber;

    std::string mName;
    std::string mInstance;

protected:
	QTcpServer* mpTcpServer = nullptr;
	QTcpSocket* mpClient = nullptr;
};