
#include "OusterStatusView.hpp"
#include "OusterModel.hpp"

#include <ouster/ouster_utils.h>


#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cOusterStatusView::cOusterStatusView(cOusterModel* pModel, QWidget* parent)
:
	cSensorStatusView(parent), mpModel(pModel)
{
	setWindowTitle("OUSTER");
}

cOusterStatusView::~cOusterStatusView()
{
}

void cOusterStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpSensorInfoValid = new QIndicator(this);
	mpSensorInfoValid->setText("Sensor Information Valid");

	mpTimeInfoValid = new QIndicator(this);
	mpTimeInfoValid->setText("Time Information Valid");

	mpLidarModeValid = new QIndicator(this);
	mpLidarModeValid->setText("LiDAR Mode Valid");

	mpBeamIntrinsicsValid = new QIndicator(this);
	mpBeamIntrinsicsValid->setText("Beam Intrinsics Valid");

	mpImuIntrinsicsValid = new QIndicator(this);
	mpImuIntrinsicsValid->setText("IMU Intrinsics Valid");

	mpLidarIntrinsicsValid = new QIndicator(this);
	mpLidarIntrinsicsValid->setText("LiDAR Intrinsics Valid");

	mpDataFormatValid = new QIndicator(this);
	mpDataFormatValid->setText("Data Format Valid");

	mpAzimuthWindowValid = new QIndicator(this);
	mpAzimuthWindowValid->setText("Azimuth Window Valid");

	mpLidarModeLabel = new QLabel("Lidar Mode:", this);
	mpLidarMode = new QLineEdit(this);
	mpLidarMode->setEnabled(false);

	mpMinAzimuthAngleLabel = new QLabel("Min (deg):", this);
	mpMinAzimuthAngle_deg = new QLineEdit(this);
	mpMinAzimuthAngle_deg->setEnabled(false);
	mpMaxAzimuthAngleLabel = new QLabel("Max (deg):", this);
	mpMaxAzimuthAngle_deg = new QLineEdit(this);
	mpMaxAzimuthAngle_deg->setEnabled(false);
}

void cOusterStatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	QGroupBox* packetBox = new QGroupBox("Received Packets");

	auto* packetInfoLayout = new QGridLayout();
	packetInfoLayout->addWidget(mpSensorInfoValid, 0, 0);
	packetInfoLayout->addWidget(mpTimeInfoValid, 0, 1);
	packetInfoLayout->addWidget(mpLidarModeValid, 0, 2);
	packetInfoLayout->addWidget(mpLidarIntrinsicsValid, 0, 3);
	packetInfoLayout->addWidget(mpBeamIntrinsicsValid, 1, 0);
	packetInfoLayout->addWidget(mpImuIntrinsicsValid, 1, 1);
	packetInfoLayout->addWidget(mpDataFormatValid, 1, 2);
	packetInfoLayout->addWidget(mpAzimuthWindowValid, 1, 3);

	packetBox->setLayout(packetInfoLayout);
	mainLayout->addWidget(packetBox);

	QGroupBox* modeBox = new QGroupBox("Operating Mode");

	auto* modeLayout = new QHBoxLayout(this);

	modeLayout->addWidget(mpLidarModeLabel);
	modeLayout->addWidget(mpLidarMode);

	modeLayout->addWidget(mpMinAzimuthAngleLabel);
	modeLayout->addWidget(mpMinAzimuthAngle_deg);
	modeLayout->addWidget(mpMaxAzimuthAngleLabel);
	modeLayout->addWidget(mpMaxAzimuthAngle_deg);

	modeBox->setLayout(modeLayout);
	mainLayout->addWidget(modeBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cOusterStatusView::onSensorInfoUpdated()
{
	mpSensorInfoValid->setState(true);
}

void cOusterStatusView::onTimeInfoUpdated()
{
	mpTimeInfoValid->setState(true);
}

void cOusterStatusView::onLidarModeUpdated()
{
	using namespace ouster;

	mpLidarModeValid->setState(true);

	auto mode = mpModel->getLidarMode();
	mpLidarMode->setText(QString::fromStdString(to_string(mode)));
}

void cOusterStatusView::onBeamIntrinsicsUpdated()
{
	mpBeamIntrinsicsValid->setState(true);
}

void cOusterStatusView::onImuIntrinsicsUpdated()
{
	mpImuIntrinsicsValid->setState(true);
}

void cOusterStatusView::onLidarIntrinsicsUpdated()
{
	mpLidarIntrinsicsValid->setState(true);
}

void cOusterStatusView::onDataFormatUpdated()
{
	mpDataFormatValid->setState(true);
}

void cOusterStatusView::onAzimuthWindowUpdated()
{
	mpAzimuthWindowValid->setState(true);

	auto window = mpModel->getAzimuthWindow();

	mpMinAzimuthAngle_deg->setText(QString::number(window.min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(window.max_deg));
}

