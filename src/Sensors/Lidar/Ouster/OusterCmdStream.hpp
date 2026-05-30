
#pragma once

#include <ouster_connect/OusterCommandStream.h>

#include <fstream>

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>


class cOusterCmdStream_Qt : public QObject, public cOusterCommandStream
{
	Q_OBJECT

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

private slots:
	void dataPending();
	void connected();
	void disconnected();
	void errorHandler(QAbstractSocket::SocketError socketError);

private:
	const size_t MAX_REPLY_LENGTH = 16 * 1024;
	bool mDataPending;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;

	std::ofstream mSocketLog;
	bool mSocketLogging = false;
};