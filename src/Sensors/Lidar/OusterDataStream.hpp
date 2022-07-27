
#pragma once

#include "ouster/OusterLidarStream.h"

#include <QByteArray>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>


class cOusterLidarStream_Qt : private cOusterLidarStream
{

public:
	cOusterLidarStream_Qt(QObject* parent = nullptr);
	~cOusterLidarStream_Qt();
	/*
	 * Determines the local endpoint to listen for the incoming data.
	 * This methods MUST be called before the moving to QThread!
	 */
	bool determineLocalEndpoint(std::string_view sensor, uint16_t port, bool use_ipv6);

	/*
	 * Starts/Stops communication with the endpoint.
	 * These methods are called inside the QThread so that
	 * all of the communication happens within the same thread!
	 */
	bool startCommunications(std::string_view sensor, uint16_t port, bool use_ipv6);
	void stopCommunications();

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

	using cOusterLidarStream::setDataFormat;

private:
	bool try_to_connect(std::string_view host, uint16_t port, bool use_ipv6) override;


	/**
	 * Maximum size for a lidar packet is for the sensors
	 * OS1-16						= 3,392
	 * OS0-32, OS1-32, OS2-32		= 6,464
	 * OS0-64, OS1-64, OS2-64		= 12,608
	 * OS0-128, OS1-128, OS2-128	= 24,896
	 */
	static const size_t MAX_DATA_LENGTH = 24896;

	QUdpSocket* mpSocket;
	QHostAddress mLocalEndpoint;
	QHostAddress mSender;
	QNetworkDatagram mDatagram;
	QByteArray mDataBuffer;
};