
#pragma once

#include <QWidget>
#include <QDialog>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QAbstractButton;
QT_END_NAMESPACE


/*******************************************************************/
/***            Base Class for Sensor Property Pages             ***/
/*******************************************************************/
class cSensorPropertyPage : public QDialog
{
	Q_OBJECT

public:
	cSensorPropertyPage(QWidget* parent = nullptr);
	virtual ~cSensorPropertyPage();

public:
	QAction* showAction() const;

	void setTitle(const QString& text);

	virtual void createWidgets() = 0;
	virtual void doLayout() = 0;

	virtual void doOK();
	virtual void doCancel();
	virtual void doApply();

signals:
	void statusMessage(QString msg);

public slots:
	virtual void showPage();

protected slots:
	void buttonClicked(QAbstractButton* button);

protected:
	QDialogButtonBox* mpButtons;

private:
	QAction* mpShowAction;
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

	std::string getHostname() const { return mHostname; }
	std::string getLocalIpAddress() const { return mLocalIpAddress; }
	uint16_t    getPort() const { return mPort; }

	bool usingIpV6() const { return mUse_IpV6; }

	bool openConnection();
	void closeConnection();

	virtual void onConnect() = 0;
	virtual void onDisconnect() {};

	/*
	 * Signals handlers from the TCP socket
	 */
private slots:
	void connected();
	void disconnected();

	virtual void errorOccurred(QAbstractSocket::SocketError socketError);
	virtual void hostFound();
	virtual void stateChanged(QAbstractSocket::SocketState socketState);

protected slots:
	void processIncomingData();

protected:
	/*
	 * Send data over the TCP socket
	 */
	int sendOutgoingData(const char* data, std::size_t len);

	/**
	 * @brief The decode method convert the byte stream from a TCP receiver port.
	 * The decode the various packets and call the various pure virtual
	 * methods.
	 */
	virtual void decodeIncomingData(const void* pBuffer, std::size_t buf_length) = 0;

protected:
	bool mConnected;

	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;

	QHostAddress mLocalEndpoint;
	QHostAddress mRemoteEndpoint;
	uint16_t   mPort;

	QTcpSocket mSocket;
	QByteArray mReplyBuffer;
};


