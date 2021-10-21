
#pragma once


#include <QByteArray>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <functional>


class cSsnxGpsStream
{
public:
//	explicit cGpsStreamSsnx(QObject* parent = nullptr);
	cSsnxGpsStream();
	~cSsnxGpsStream();

	void registerDataProcessingCallback(std::function<void(const void* pBuffer, std::size_t buf_length)> fp);

	/**
	 * Try to setup the socket to listen for GPS data
	 */
	bool isConnected() const;

	/**
	 * Try to setup the socket to listen for GPS data
	 */
	bool try_to_connect(std::string_view host, uint16_t port, bool use_ipv6);

	/**
	 * Clear will remove all input datagrams from the UDP socket
	 */
	void clear();

	/**
	 * receive_data will retrieve and process datagrams from the UDP socket.
	 * Note: call this method in a non-threaded non-GUI console application
	 */
	void receive_data();

	/**
	 * processOneDatagram will retrieve and process a single datagram from the UDP socket.
	 * Note: this method will return immediately if no datagram is waiting
	 */
	void processOneDatagram();
	void processDatagrams();

	virtual void processDatagram(const void* pBuffer, std::size_t buf_length) = 0;

private:
	static const size_t MAX_DATA_LENGTH = 128;

	QUdpSocket mSocket;
	QHostAddress mSender;
	QNetworkDatagram mDatagram;
	QByteArray mDataBuffer;

	std::function<void(const void* pBuffer, std::size_t buf_length)> mProcessingCallback;
};

