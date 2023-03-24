
#pragma once

#include <spidercam/spidercam_com.hpp>

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>


class cSpidercamController : public QObject, public cSpiderCamCom
{
	Q_OBJECT

public:
	cSpidercamController(QObject* parent = nullptr);
	~cSpidercamController();

	bool hasRemoteEndpoint() const;
	QString remoteEndpoint() const;
	uint16_t remotePort() const;

	bool isConnected() const;

	bool try_to_connect(std::string_view hostname, uint16_t port, std::string_view local_ip, bool use_ipv6 = false);

	bool startCommunications();
	void stopCommunications();

	void clearIncomingBuffer() override;
	bool checkForReply() override;

signals:
	void connectionStateChange(bool connected);

private slots:
	void onConnect();
	void onDisconnect();
	void errorHandler(QAbstractSocket::SocketError socketError);

private:
	int send_cmd(const std::string_view msg) override;
	std::string recv_reply() override;

	const size_t MAX_REPLY_LENGTH = 16 * 1024;

	QHostAddress mLocalEndpoint;
	QHostAddress mRemoteEndpoint;
	uint16_t mPort;

	QTcpSocket* mpSocket;
	QByteArray mReplyBuffer;
};