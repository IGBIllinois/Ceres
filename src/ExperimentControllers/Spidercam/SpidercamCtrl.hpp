
#pragma once

#include <spidercam_connect/spidercam_com.hpp>

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>


class cSpidercamController : public QObject, public cSpiderCamCom
{
	Q_OBJECT

public:
	cSpidercamController(QObject* parent = nullptr);
	virtual ~cSpidercamController();

	double getComputedVx_mmps() const;
	double getComputedVy_mmps() const;
	double getComputedVz_mmps() const;

	bool sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
		uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg) override;

public:
	bool hasRemoteEndpoint() const;
	QString remoteEndpoint() const;
	uint16_t remotePort() const;

	bool isConnected() const;

	bool try_to_connect(std::string_view hostname, uint16_t port, std::string_view local_ip, bool use_ipv6 = false);

	bool startCommunications();
	void stopCommunications();

	void clearIncomingBuffer() override;
	bool checkForReply() override;

signals:
	void connectionStateChange(bool connected);

private slots:
	void onConnect();
	void onDisconnect();
	void errorHandler(QAbstractSocket::SocketError socketError);

protected:
	double mVx_mmps = 0.0;
	double mVy_mmps = 0.0;
	double mVz_mmps = 0.0;

private:
	int send_cmd(const std::string_view msg) override;
	std::string recv_reply() override;

	const size_t MAX_REPLY_LENGTH = 16 * 1024;

	QHostAddress mLocalEndpoint;
	QHostAddress mRemoteEndpoint;
	uint16_t mPort = 0;

	QTcpSocket* mpSocket;
	QByteArray mReplyBuffer;
};


inline double cSpidercamController::getComputedVx_mmps() const { return mVx_mmps; }
inline double cSpidercamController::getComputedVy_mmps() const { return mVy_mmps; }
inline double cSpidercamController::getComputedVz_mmps() const { return mVz_mmps; }
