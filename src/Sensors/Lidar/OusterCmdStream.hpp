
#pragma once

#include "ouster/OusterCommandStream.h"

#include <QByteArray>
#include <QtNetwork/QTcpSocket>


class cOusterCmdStream_Qt : public cOusterCommandStream
{
public:
	cOusterCmdStream_Qt();
	~cOusterCmdStream_Qt();

private:
	bool try_to_connect(std::string_view hostname, uint16_t port, bool use_ipv6) override;
	int send_cmd(const std::string_view msg) override;
	std::string recv_reply() override;

	void errorHandler(QAbstractSocket::SocketError socketError);

	const size_t MAX_REPLY_LENGTH = 16 * 1024;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;
};