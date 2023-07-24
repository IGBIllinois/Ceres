
#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include <nlohmann/json.hpp>


/*******************************************************************/
/**   Interface for Experiment State to Control Remote Sensor     **/
/*******************************************************************/
class cExperimentStateRemoteInterface : public QObject
{
	Q_OBJECT

public:
	cExperimentStateRemoteInterface(QObject* parent = nullptr);
	virtual ~cExperimentStateRemoteInterface();

	bool initialize(const std::string& hostname, uint16_t port,
		bool use_ipv6, const std::string& local_ip);

	void destroy();

	std::string getHostname() const { return mHostname; }
	std::string getLocalIpAddress() const { return mLocalIpAddress; }
	uint16_t    getPort() const { return mPort; }

	bool usingIpV6() const { return mUse_IpV6; }

	bool openConnection();
	void closeConnection();

	virtual void onConnect() = 0;
	virtual void onDisconnect() {};

	/*
	 * Signals handlers from the TCP socket
	 */
private slots:
	void connected();
	void disconnected();
	void errorOccurred(QAbstractSocket::SocketError socketError);
	void hostFound();
	void stateChanged(QAbstractSocket::SocketState socketState);
	void processIncomingData();

protected:
	/*
	 * Send data over the TCP socket
	 */
	int sendOutgoingData(const char* data, std::size_t len);

	/**
	 * @brief The decode method convert the byte stream from a TCP receiver port.
	 * The decode the various packets and call the various pure virtual
	 * methods.
	 */
	virtual void decodeIncomingData(const void* pBuffer, std::size_t buf_length) = 0;

protected:
	bool mConnected;

	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;

	QHostAddress mLocalEndpoint;
	QHostAddress mRemoteEndpoint;
	uint16_t   mPort;

	QTcpSocket* mpSocket;
	QByteArray mReplyBuffer;
};


