
#pragma once

#include "ouster/OusterLidarStream.h"

#include <QByteArray>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>


class cOusterLidarStream_Qt : public QObject, public cOusterLidarStream
{
	Q_OBJECT

public:
	explicit cOusterLidarStream_Qt(QObject* parent = nullptr);
	~cOusterLidarStream_Qt();
	

	/**
	 * Clear will remove all input datagrams from the UDP socket
	 */
	void clear();

	/**
	 * receive_data will retrieve and process  datagrams from the UDP socket.
	 * Note: call this method in a non-threaded non-GUI console application
	 */
	void receive_data();

signals:

	void dataUpdated(uint16_t frameID, ouster::lidar_data_t data);

private:
	bool try_to_connect(std::string_view host, uint16_t port, bool use_ipv6) override;
	void processDatagram();

	void onNewData(uint16_t frameID, ouster::lidar_data_t& data) override;


	/**
	 * Maximum size for a lidar packet is for the sensors
	 * OS1-16						= 3,392
	 * OS0-32, OS1-32, OS2-32		= 6,464
	 * OS0-64, OS1-64, OS2-64		= 12,608
	 * OS0-128, OS1-128, OS2-128	= 24,896
	 */
	static const size_t MAX_DATA_LENGTH = 24896;

	QUdpSocket mSocket;
	QHostAddress mSender;
	QNetworkDatagram mDatagram;
	QByteArray mDataBuffer;
};