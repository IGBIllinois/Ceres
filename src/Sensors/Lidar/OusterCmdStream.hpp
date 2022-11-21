
#pragma once

#include "ouster/OusterCommandStream.h"

#include <fstream>

#include <QByteArray>
#include <QtNetwork/QTcpSocket>


class cOusterCmdStream_Qt : public cOusterCommandStream
{
public:
	cOusterCmdStream_Qt(QObject* parent = nullptr);
	~cOusterCmdStream_Qt();

	void enableSocketLogging();
	void disableSocketLogging();

private:
	bool try_to_connect(std::string_view hostname, uint16_t port, 
						bool use_ipv6, std::string_view local_ip) override;

	int send_cmd(const std::string_view msg) override;
	bool recv_data_available() override;
	std::string recv_reply() override;
	std::string recv_json_reply() override;

	void flush();

	void errorHandler(QAbstractSocket::SocketError socketError);

	const size_t MAX_REPLY_LENGTH = 16 * 1024;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;

	std::ofstream mSocketLog;
	bool mSocketLogging = false;
};