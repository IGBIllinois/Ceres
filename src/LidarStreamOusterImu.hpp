
#pragma once

#include "ouster/OusterImuStream.h"

#include <QByteArray>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>


class cOusterImuStream_Qt : public QObject, public cOusterImuStream
{
	Q_OBJECT

public:
	explicit cOusterImuStream_Qt(QObject* parent = nullptr);
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

private:
	bool try_to_connect(std::string_view host, uint16_t port, bool use_ipv6) override;
	void processDatagram();

signals:
	void dataUpdated(ouster::imu_data_t new_data);

private:
	void onNewData(const ouster::imu_data_t& new_data) override;

private:
	static const size_t MAX_DATA_LENGTH = 128;

	QUdpSocket mSocket;
	QHostAddress mSender;
	QNetworkDatagram mDatagram;
	QByteArray mDataBuffer;
};

