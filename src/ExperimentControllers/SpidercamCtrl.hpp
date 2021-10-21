
#pragma once

#include <spidercam/spidercam_com.hpp>

#include <QByteArray>
#include <QtNetwork/QTcpSocket>


class cSpidercamController : public cSpiderCamCom
{
public:
	cSpidercamController();
	~cSpidercamController();

	bool try_to_connect(std::string_view hostname, uint16_t port, bool use_ipv6 = false);

	void clearIncomingBuffer() override;
	bool checkForReply() override;

private:
	int send_cmd(const std::string_view msg) override;
	std::string recv_reply() override;

	void errorHandler(QAbstractSocket::SocketError socketError);

	const size_t MAX_REPLY_LENGTH = 16 * 1024;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;
};