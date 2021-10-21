
#pragma once

#include "ouster/OusterImuStream.h"

#include <QByteArray>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>


class cOusterImuStream_Qt : public cOusterImuStream
{
public:
	cOusterImuStream_Qt();
	~cOusterImuStream_Qt();

	/**
	 * Clear will remove all input datagrams from the UDP socket
	 */
	void clear();

	/**
	 * receive_data will retrieve and process  datagrams from the UDP socket.
	 * Note: call this method in a non-threaded non-GUI console application
	 */
	void receive_data();
	void processOneDatagram();
	void processDatagrams();

private:
	bool try_to_connect(std::string_view host, uint16_t port, bool use_ipv6) override;

private:
	static const size_t MAX_DATA_LENGTH = 128;

	QUdpSocket mSocket;
	QHostAddress mSender;
	QNetworkDatagram mDatagram;
	QByteArray mDataBuffer;
};

