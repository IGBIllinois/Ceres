
#include "SsnxStatusView.hpp"
#include "SsnxModel.hpp"
#include "../Sensors/SensorModel.hpp"
#include "Constants.hpp"
#include "QIndicator.hpp"

#include "../GpsUtils.hpp"

#include <ssnx/ssn_utils.hpp>

#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>

constexpr int MIN_VALUE = -20000;

cSsnxStatusView::cSsnxStatusView(cSsnxModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent)
{}

cSsnxStatusView::~cSsnxStatusView()
{
}


void cSsnxStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpPvtCartesianValid = new QLedIndicator("Cartesian Position/Velocity/Time Valid", this);
	mpPvtGeodeticValid  = new QLedIndicator("Geodetic Position/Velocity/Time Valid", this);

//	mpPosCovGeodeticValid = new QLedIndicator("Geodetic Position Covariance Matrix Valid", this);
//	mpVelCovGeodeticValid = new QLedIndicator("Geodetic Velocity Covariance Matrix Valid", this);

	mpPosProjectedValid = new QLedIndicator("Projected Position Valid", this);
	mpReceiverTimeValid = new QLedIndicator("Receiver Time Valid", this);

	mpRtcmDatumValid = new QLedIndicator("RTCM Datum Valid", this);
	mpWifiClientValid = new QLedIndicator("Wifi Client Valid", this);
	mpNtripClientValid = new QLedIndicator("NTRIP Client Info Valid", this);
	mpReceiverStatusValid = new QLedIndicator("Receiver Status Valid", this);

	mpRxErrorCongestion = new QLedIndicator("Congestion", this);
	mpRxErrorCpuOverload = new QLedIndicator("Cpu Overload", this);
	mpRxErrorInvalidConfig = new QLedIndicator("Invalid Config", this);
	mpRxErrorMissedEvent = new QLedIndicator("Missed Event", this);
	mpRxErrorOutOfGeoFence = new QLedIndicator("Out Of Geo Fence", this);
	mpRxErrorSoftware = new QLedIndicator("Software", this);
	mpRxErrorWatchdog = new QLedIndicator("Watchdog", this);

	mpRxErrorCongestion->setLedSize(8);
	mpRxErrorCpuOverload->setLedSize(8);
	mpRxErrorInvalidConfig->setLedSize(8);
	mpRxErrorMissedEvent->setLedSize(8);
	mpRxErrorOutOfGeoFence->setLedSize(8);
	mpRxErrorSoftware->setLedSize(8);
	mpRxErrorWatchdog->setLedSize(8);

	mpRxErrorCongestion->setOnColor(Qt::red);
	mpRxErrorCpuOverload->setOnColor(Qt::red);
	mpRxErrorInvalidConfig->setOnColor(Qt::red);
	mpRxErrorMissedEvent->setOnColor(Qt::red);
	mpRxErrorOutOfGeoFence->setOnColor(Qt::red);
	mpRxErrorSoftware->setOnColor(Qt::red);
	mpRxErrorWatchdog->setOnColor(Qt::red);

	mpRxErrorCongestion->setOffColor(Qt::transparent);
	mpRxErrorCpuOverload->setOffColor(Qt::transparent);
	mpRxErrorInvalidConfig->setOffColor(Qt::transparent);
	mpRxErrorMissedEvent->setOffColor(Qt::transparent);
	mpRxErrorOutOfGeoFence->setOffColor(Qt::transparent);
	mpRxErrorSoftware->setOffColor(Qt::transparent);
	mpRxErrorWatchdog->setOffColor(Qt::transparent);


	mpNtripState = new QLineEdit();
	mpNtripState->setReadOnly(true);

	mpNtripErrorCode = new QLineEdit();
	mpNtripErrorCode->setReadOnly(true);

	mpWifiConnectionStatus = new QMultiStateLedIndicator("Connection", this);
	mpWifiConnectionStatus->setStateStyle(0, Qt::red, Qt::SolidPattern);
	mpWifiConnectionStatus->setStateStyle(1, Qt::yellow, Qt::SolidPattern);
	mpWifiConnectionStatus->setStateStyle(2, Qt::green, Qt::SolidPattern);

	mpWifiPowerLevel_dBm = new QLineEdit();
	mpWifiPowerLevel_dBm->setReadOnly(true);
	mpWifiPowerLevel_dBm->setMaxLength(100);

	mpWifiErrorCode = new QLineEdit();
	mpWifiErrorCode->setReadOnly(true);

	mpLatitude_deg = new QLineEdit();
	mpLatitude_deg->setReadOnly(true);

	mpLongitude_deg = new QLineEdit();
	mpLongitude_deg->setReadOnly(true);

	mpHeight_m = new QLineEdit();
	mpHeight_m->setReadOnly(true);

	mpNorthVelocity_mps = new QLineEdit();
	mpNorthVelocity_mps->setReadOnly(true);

	mpEastVelocity_mps = new QLineEdit();
	mpEastVelocity_mps->setReadOnly(true);

	mpUpVelocity_mps = new QLineEdit();
	mpUpVelocity_mps->setReadOnly(true);

	mpGroundTrack_deg = new QLineEdit();
	mpGroundTrack_deg->setReadOnly(true);

	mpDatum = new QLineEdit();
	mpDatum->setReadOnly(true);

	mpNumBases = new QLineEdit();
	mpNumBases->setReadOnly(true);

	mpNumSV = new QLineEdit();
	mpNumSV->setReadOnly(true);

	mpSolutionType = new QLineEdit();
	mpSolutionType->setReadOnly(true);

	mpTimestamp_s = new QLineEdit();
	mpTimestamp_s->setReadOnly(true);

	mpDate = new QLineEdit();
	mpDate->setReadOnly(true);

	mpTime = new QLineEdit();
	mpTime->setReadOnly(true);

	mpX_mm = new QLineEdit(this);
	mpX_mm->setReadOnly(true);

	mpY_mm = new QLineEdit(this);
	mpY_mm->setReadOnly(true);

	mpZ_mm = new QLineEdit(this);
	mpZ_mm->setReadOnly(true);

	mpRef_X_mm = new QLineEdit(this);
	mpRef_X_mm->setReadOnly(true);

	mpRef_Y_mm = new QLineEdit(this);
	mpRef_Y_mm->setReadOnly(true);

	mpRef_Z_mm = new QLineEdit(this);
	mpRef_Z_mm->setReadOnly(true);

	mpRef_Error_mm = new QLineEdit(this);
	mpRef_Error_mm->setReadOnly(true);

	mpRef_Count = new QLineEdit(this);
	mpRef_Count->setReadOnly(true);

	mpReconnect = new QPushButton("Reconnect", this);
	mpReconnect->setEnabled(false);
	connect(mpReconnect, &QPushButton::clicked, this, &cSsnxStatusView::tryGpsReconnection);
}

void cSsnxStatusView::doLayout()
{
	QLabel* text = nullptr;

	auto* mainLayout = new QVBoxLayout(this);

	auto* sensorLayout = new QHBoxLayout(this);

	sensorLayout->addWidget(getSensorStatusBox(), 1);
	sensorLayout->addSpacing(10);
	sensorLayout->addWidget(mpReconnect);

	mainLayout->addLayout(sensorLayout);

	QGroupBox* packetBox = new QGroupBox("Received Packets");

	auto* packetInfoLayout = new QGridLayout();
	packetInfoLayout->addWidget(mpPvtCartesianValid, 0, 0);
	packetInfoLayout->addWidget(mpPvtGeodeticValid, 0, 1);
//	packetInfoLayout->addWidget(mpPosCovGeodeticValid, 0, 2);
//	packetInfoLayout->addWidget(mpVelCovGeodeticValid, 0, 3);
	packetInfoLayout->addWidget(mpReceiverStatusValid, 0, 2);
	packetInfoLayout->addWidget(mpWifiClientValid, 0, 3);
	packetInfoLayout->addWidget(mpNtripClientValid, 0, 4);
	packetInfoLayout->addWidget(mpPosProjectedValid, 1, 0);
	packetInfoLayout->addWidget(mpReceiverTimeValid, 1, 1);
	packetInfoLayout->addWidget(mpRtcmDatumValid, 1, 2);

	packetBox->setLayout(packetInfoLayout);

	mainLayout->addWidget(packetBox);

	mainLayout->addSpacing(5);

	auto* statusLayout = new QHBoxLayout();

	QGroupBox* receiverBox = new QGroupBox("Receiver Information");

	auto* rxErrorLayout = new QGridLayout();

	rxErrorLayout->addWidget(mpRxErrorCongestion, 0, 0);
	rxErrorLayout->addWidget(mpRxErrorCpuOverload, 0, 1);
	rxErrorLayout->addWidget(mpRxErrorInvalidConfig, 0, 2);
	rxErrorLayout->addWidget(mpRxErrorMissedEvent, 0, 3);
	rxErrorLayout->addWidget(mpRxErrorOutOfGeoFence, 1, 0);
	rxErrorLayout->addWidget(mpRxErrorSoftware, 1, 1);
	rxErrorLayout->addWidget(mpRxErrorWatchdog, 1, 2);

	receiverBox->setLayout(rxErrorLayout);
	statusLayout->addWidget(receiverBox);
	statusLayout->addSpacing(5);

	QGroupBox* wifiBox = new QGroupBox("Wifi Information");

	auto* wifiLayout = new QVBoxLayout();

	auto* wifiConnectLayout = new QHBoxLayout();

	wifiConnectLayout->addWidget(mpWifiConnectionStatus);
	text = new QLabel("Power Level (dBm)");
	wifiConnectLayout->addWidget(text);
	wifiConnectLayout->addWidget(mpWifiPowerLevel_dBm);

	wifiLayout->addLayout(wifiConnectLayout);

	auto* wifiErrorLayout = new QHBoxLayout();

	text = new QLabel("Error Code");
	wifiErrorLayout->addWidget(text);
	wifiErrorLayout->addWidget(mpWifiErrorCode, 1);

	wifiLayout->addLayout(wifiErrorLayout);

	wifiBox->setLayout(wifiLayout);
	statusLayout->addWidget(wifiBox);
	statusLayout->addSpacing(5);

	QGroupBox* ntripBox = new QGroupBox("NTRIP Client Information");

	auto* ntripLayout = new QGridLayout();

	text = new QLabel("Status");
	ntripLayout->addWidget(text, 0, 0);
	ntripLayout->addWidget(mpNtripState, 0, 1);
	text = new QLabel("Error Code");
	ntripLayout->addWidget(text, 1, 0);
	ntripLayout->addWidget(mpNtripErrorCode, 1, 1);

	ntripBox->setLayout(ntripLayout);
	statusLayout->addWidget(ntripBox);

	mainLayout->addLayout(statusLayout);

	mainLayout->addSpacing(5);

	QGroupBox* geodeticBox = new QGroupBox("Geodetic Position");

	auto* pLatLngLayout = new QHBoxLayout();

	text = new QLabel("Latitude (deg)");
	pLatLngLayout->addWidget(text);
	pLatLngLayout->addWidget(mpLatitude_deg, 1);
	pLatLngLayout->addSpacing(10);

	text = new QLabel("Longitude (deg)");
	pLatLngLayout->addWidget(text);
	pLatLngLayout->addWidget(mpLongitude_deg, 1);
	pLatLngLayout->addSpacing(10);

	text = new QLabel("Height (m)");
	pLatLngLayout->addWidget(text);
	pLatLngLayout->addWidget(mpHeight_m, 1);

	geodeticBox->setLayout(pLatLngLayout);

	mainLayout->addWidget(geodeticBox);

	mainLayout->addSpacing(5);

	QGroupBox* speedBox = new QGroupBox("GPS Speeds");

	auto* pSpeedLayout = new QHBoxLayout();

	text = new QLabel("North Speed (m/s)");
	pSpeedLayout->addWidget(text);
	pSpeedLayout->addWidget(mpNorthVelocity_mps, 1);
	pSpeedLayout->addSpacing(10);

	text = new QLabel("East Speed (m/s)");
	pSpeedLayout->addWidget(text);
	pSpeedLayout->addWidget(mpEastVelocity_mps, 1);
	pSpeedLayout->addSpacing(10);

	text = new QLabel("Vertical Speed (m/s)");
	pSpeedLayout->addWidget(text);
	pSpeedLayout->addWidget(mpUpVelocity_mps, 1);

	speedBox->setLayout(pSpeedLayout);

	mainLayout->addWidget(speedBox);

	mainLayout->addSpacing(5);

	QGroupBox* infoBox = new QGroupBox("GPS Information");

	auto* pInfoLayout = new QHBoxLayout();

	text = new QLabel("Ground Track (deg)");
	pInfoLayout->addWidget(text);
	pInfoLayout->addWidget(mpGroundTrack_deg, 1);
	pInfoLayout->addSpacing(10);

	text = new QLabel("Datum");
	pInfoLayout->addWidget(text);
	pInfoLayout->addWidget(mpDatum, 1);
	pInfoLayout->addSpacing(10);

	text = new QLabel("Num Of Satellites");
	pInfoLayout->addWidget(text);
	pInfoLayout->addWidget(mpNumSV, 1);
	pInfoLayout->addSpacing(10);

	text = new QLabel("Num Of Bases");
	pInfoLayout->addWidget(text);
	pInfoLayout->addWidget(mpNumBases, 1);
	pInfoLayout->addSpacing(10);

	text = new QLabel("Solution Type");
	pInfoLayout->addWidget(text);
	pInfoLayout->addWidget(mpSolutionType, 1);

	infoBox->setLayout(pInfoLayout);

	mainLayout->addWidget(infoBox);

	mainLayout->addSpacing(5);

	QGroupBox* timeBox = new QGroupBox("GPS Time");

	auto* pTimeLayout = new QHBoxLayout();

	text = new QLabel("Timestamp (s):");
	pTimeLayout->addWidget(text);
	pTimeLayout->addWidget(mpTimestamp_s, 1);
	pTimeLayout->addSpacing(10);

	text = new QLabel("Date:");
	pTimeLayout->addWidget(text);
	pTimeLayout->addWidget(mpDate, 1);
	pTimeLayout->addSpacing(10);

	text = new QLabel("Time:");
	pTimeLayout->addWidget(text);
	pTimeLayout->addWidget(mpTime, 1);

	timeBox->setLayout(pTimeLayout);

	mainLayout->addWidget(timeBox);

	mainLayout->addSpacing(5);

	QGroupBox* posBox = new QGroupBox("Approximate Spidercam Position");

	auto* pPosLayout = new QHBoxLayout();
	text = new QLabel("X (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpX_mm, 1);
	pPosLayout->addSpacing(10);

	text = new QLabel("Y (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpY_mm, 1);
	pPosLayout->addSpacing(10);

	text = new QLabel("Z (mm)", this);
	pPosLayout->addWidget(text);
	pPosLayout->addWidget(mpZ_mm, 1);

	posBox->setLayout(pPosLayout);

	mainLayout->addWidget(posBox);

	mainLayout->addSpacing(5);

	QGroupBox* refBox = new QGroupBox("Reference Position");

	auto* pRefLayout = new QHBoxLayout();
	text = new QLabel("X (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_X_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Y (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Y_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Z (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Z_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Error (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Error_mm, 1);

	text = new QLabel("Count", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Count, 1);

	refBox->setLayout(pRefLayout);

	mainLayout->addWidget(refBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cSsnxStatusView::onSensorStatusChange(QString name, QString instance, sensor::eStatus status)
{
	mpReconnect->setDisabled(status == sensor::eStatus::RUNNING);

	cSensorStatusView::onSensorStatusChange(name, instance, status);
}

void cSsnxStatusView::onPvtCartesianStateChange(bool valid)
{
	mpPvtCartesianValid->setState(valid);
}

void cSsnxStatusView::onPvtGeodeticStateChange(bool valid)
{
	mpPvtGeodeticValid->setState(valid);
}

void cSsnxStatusView::onPosCovGeodeticStateChange(bool valid)
{
//	mpPosCovGeodeticValid->setState(valid);
}

void cSsnxStatusView::onVelCovGeodeticStateChange(bool valid)
{
//	mpVelCovGeodeticValid->setState(valid);
}

void cSsnxStatusView::onPosProjectedStateChange(bool valid)
{
	mpPosProjectedValid->setState(valid);
}

void cSsnxStatusView::onReceiverTimeStateChange(bool valid)
{
	mpReceiverTimeValid->setState(valid);
}

void cSsnxStatusView::onRtcmDatumStateChange(bool valid)
{
	mpRtcmDatumValid->setState(valid);
}

void cSsnxStatusView::onReceiverStateChange(int error_code)
{
	mpRxErrorCongestion->setState(error_code & 0x01);
	mpRxErrorCpuOverload->setState(error_code & 0x02);
	mpRxErrorInvalidConfig->setState(error_code & 0x04);
	mpRxErrorMissedEvent->setState(error_code & 0x08);
	mpRxErrorOutOfGeoFence->setState(error_code & 0x10);
	mpRxErrorSoftware->setState(error_code & 0x20);
	mpRxErrorWatchdog->setState(error_code & 0x40);
}

void cSsnxStatusView::onNtripClientChange(int state, int errorCode)
{
	mpNtripState->setText(QString::fromStdString(ssnx::to_string(ssnx::to_ntrip_status(state))));
	mpNtripErrorCode->setText(QString::fromStdString(ssnx::to_string(ssnx::to_ntrip_error_code(errorCode))));
}

void cSsnxStatusView::onWifiConnectionChange(int state, int powerLevel_dBm, int errorCode)
{
	mpWifiConnectionStatus->changeState(state);

	if (powerLevel_dBm < -127)
		mpWifiPowerLevel_dBm->setText("---");
	else
		mpWifiPowerLevel_dBm->setText(QString::number(powerLevel_dBm));

	mpWifiErrorCode->setText(QString::fromStdString(ssnx::to_string(ssnx::to_wifi_error_code(errorCode))));
}

void cSsnxStatusView::onNtripStateChange(bool valid)
{
	mpNtripClientValid->setState(valid);
}

void cSsnxStatusView::onWifiClientStateChange(bool valid)
{
	mpWifiClientValid->setState(valid);
}

void cSsnxStatusView::onReceiverStatusStateChange(bool valid)
{
	mpReceiverStatusValid->setState(valid);
}

void cSsnxStatusView::onGeodeticPVT_Change(double timestamp_s,
	double lat_rad, double lng_rad, double height_m,
	double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
	double groundTrack_deg, int datum, int num_sv, int num_bases)
{
	if (!isActiveWindow())
		return;

	mpLatitude_deg->setText(QString::number(lat_rad * nConstants::RAD_TO_DEG, 'f', 10));
	mpLongitude_deg->setText(QString::number(lng_rad * nConstants::RAD_TO_DEG, 'f', 10));
	mpHeight_m->setText(QString::number(height_m));
	mpNorthVelocity_mps->setText(QString::number(northSpeed_mps));
	mpEastVelocity_mps->setText(QString::number(eastSpeed_mps));
	mpUpVelocity_mps->setText(QString::number(vertSpeed_mps));

	if ((groundTrack_deg > 360.0) || (groundTrack_deg < -360.0))
		mpGroundTrack_deg->setText("");
	else
		mpGroundTrack_deg->setText(QString::number(groundTrack_deg));

	mpTimestamp_s->setText(QString::number(timestamp_s, 'f', 6));
	mpDatum->setText(::gps::to_qstring(::gps::to_datum(datum)));
	mpNumSV->setText(QString::number(num_sv));
	mpNumBases->setText(QString::number(num_bases));
}


void cSsnxStatusView::onUTC_Change(int hour, int min, int sec, int day, int month, int year)
{
	if (!isActiveWindow())
		return;

	mpDate->setText(QString("%1/%2/%3").arg(QString::number(month), 2, '0')
		.arg(QString::number(day), 2, '0').arg(QString::number(year)));

	mpTime->setText(QString("%1:%2:%3").arg(QString::number(hour), 2, '0')
		.arg(QString::number(min), 2, '0').arg(QString::number(sec), 2, '0'));
}

void cSsnxStatusView::onSolutionTypeChange(int solution_type)
{
	if (!isActiveWindow())
		return;

	mpSolutionType->setText(::gps::to_qstring(::gps::to_solution_type(solution_type)));
}

void cSsnxStatusView::onPositionChange(int x_mm, int y_mm, int z_mm)
{
	if (!isActiveWindow())
		return;

	if (x_mm < MIN_VALUE)
		mpX_mm->setText("");
	else
		mpX_mm->setText(QString::number(x_mm));

	if (y_mm < MIN_VALUE)
		mpY_mm->setText("");
	else
		mpY_mm->setText(QString::number(y_mm));

	if (z_mm < MIN_VALUE)
		mpZ_mm->setText("");
	else
		mpZ_mm->setText(QString::number(z_mm));
}

void cSsnxStatusView::onReferencePositionChange(int x_mm, int y_mm, int z_mm, double error_mm, int count)
{
	if (!isActiveWindow())
		return;

	if (x_mm < MIN_VALUE)
		mpRef_X_mm->setText("");
	else
		mpRef_X_mm->setText(QString::number(x_mm));

	if (y_mm < MIN_VALUE)
		mpRef_Y_mm->setText("");
	else
		mpRef_Y_mm->setText(QString::number(y_mm));

	if (z_mm < MIN_VALUE)
		mpRef_Z_mm->setText("");
	else
		mpRef_Z_mm->setText(QString::number(z_mm));

	if (error_mm < 0)
		mpRef_Error_mm->setText("");
	else
		mpRef_Error_mm->setText(QString::number(error_mm));

	if (count < 0)
		mpRef_Count->setText("");
	else
		mpRef_Count->setText(QString::number(count));
}

