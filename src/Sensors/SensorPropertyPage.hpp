
#pragma once

#include <QWidget>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
QT_END_NAMESPACE


/*******************************************************************/
/***            Base Class for Sensor Property Pages             ***/
/*******************************************************************/
class cSensorPropertyPage : public QWidget
{
	Q_OBJECT

public:
	cSensorPropertyPage(QWidget* parent = nullptr);
	virtual ~cSensorPropertyPage();

public:
	QAction* showAction() const;

	void setTitle(const QString& text);

public slots:
	void showPage();

private:
	QAction* mpShowAction;
	QDialogButtonBox* mpButtons;
};


/*******************************************************************/
/** Interface for Remote Sensor Property Pages for Remote Sensor  **/
/*******************************************************************/
class cSensorPropertyPageRemoteInterface : public QObject
{
	Q_OBJECT

public:
	cSensorPropertyPageRemoteInterface(QObject* parent = nullptr);
	virtual ~cSensorPropertyPageRemoteInterface();

	bool initialize(const std::string& hostname, uint16_t port,
		bool use_ipv6, const std::string& local_ip);

	/*
	 * Signals handlers from the TCP socket
	 */
private slots:
	void connected();
	void disconnected();
	void errorOccurred(QAbstractSocket::SocketError socketError);
	void hostFound();
	void stateChanged(QAbstractSocket::SocketState socketState);
	void processNewCommand();

	/*
	 * Send data over the TCP socket
	 */
protected:
	int sendOutgoingData(const char* data, std::size_t len);

protected:
	bool mConnected;

	QHostAddress mLocalEndpoint;
	QHostAddress mRemoteEndpoint;
	uint16_t   mPort;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;
};


