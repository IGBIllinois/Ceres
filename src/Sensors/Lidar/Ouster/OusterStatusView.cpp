
#include "OusterStatusView.hpp"
#include "OusterModel.hpp"
#include "QIndicator.hpp"

#include <ouster_connect/ouster_utils.h>

#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cOusterStatusView::cOusterStatusView(cOusterModel* pModel, QWidget* parent)
:
	cSensorStatusView(pModel, parent), mpModel(pModel)
{
}

cOusterStatusView::~cOusterStatusView()
{
}

void cOusterStatusView::createWidgets()
{
	cSensorStatusView::createWidgets();

	mpSensorInfoValid = new QLedIndicator(this);
	mpSensorInfoValid->setText("Sensor Information Valid");

	mpTimeInfoValid = new QLedIndicator(this);
	mpTimeInfoValid->setText("Time Information Valid");

	mpLidarModeValid = new QLedIndicator(this);
	mpLidarModeValid->setText("LiDAR Mode Valid");

	mpBeamIntrinsicsValid = new QLedIndicator(this);
	mpBeamIntrinsicsValid->setText("Beam Intrinsics Valid");

	mpImuIntrinsicsValid = new QLedIndicator(this);
	mpImuIntrinsicsValid->setText("IMU Intrinsics Valid");

	mpLidarIntrinsicsValid = new QLedIndicator(this);
	mpLidarIntrinsicsValid->setText("LiDAR Intrinsics Valid");

	mpDataFormatValid = new QLedIndicator(this);
	mpDataFormatValid->setText("Data Format Valid");

	mpAzimuthWindowValid = new QLedIndicator(this);
	mpAzimuthWindowValid->setText("Azimuth Window Valid");

	mpLidarModeLabel = new QLabel("Lidar Mode:", this);
	mpLidarMode = new QLineEdit(this);
	mpLidarMode->setReadOnly(true);

	mpRangeLabel = new QLabel("Range to Ground (mm):", this);
	mpRange_mm = new QLineEdit(this);
	mpRange_mm->setReadOnly(true);

	mpMinAzimuthAngleLabel = new QLabel("Min (deg):", this);
	mpMinAzimuthAngle_deg = new QLineEdit(this);
	mpMinAzimuthAngle_deg->setReadOnly(true);
	mpMaxAzimuthAngleLabel = new QLabel("Max (deg):", this);
	mpMaxAzimuthAngle_deg = new QLineEdit(this);
	mpMaxAzimuthAngle_deg->setReadOnly(true);
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

	auto* modeLayout = new QHBoxLayout();

	modeLayout->addWidget(mpLidarModeLabel);
	modeLayout->addWidget(mpLidarMode);

	modeLayout->addWidget(mpMinAzimuthAngleLabel);
	modeLayout->addWidget(mpMinAzimuthAngle_deg);
	modeLayout->addWidget(mpMaxAzimuthAngleLabel);
	modeLayout->addWidget(mpMaxAzimuthAngle_deg);

	modeBox->setLayout(modeLayout);
	mainLayout->addWidget(modeBox);

	QGroupBox* rangeBox = new QGroupBox("Measurement Info");

	auto* rangeLayout = new QHBoxLayout();

	rangeLayout->addWidget(mpRangeLabel);
	rangeLayout->addWidget(mpRange_mm);

	rangeBox->setLayout(rangeLayout);
	mainLayout->addWidget(rangeBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cOusterStatusView::connectToModel()
{
	connect(mpModel, &cOusterModel::updateSensorInfo,      this, &cOusterStatusView::onSensorInfoUpdated);
	connect(mpModel, &cOusterModel::updateTimeInfo,        this, &cOusterStatusView::onTimeInfoUpdated);
	connect(mpModel, &cOusterModel::updateLidarMode,       this, &cOusterStatusView::onLidarModeUpdated);
	connect(mpModel, &cOusterModel::updateBeamIntrinsics,  this, &cOusterStatusView::onBeamIntrinsicsUpdated);
	connect(mpModel, &cOusterModel::updateImuIntrinsics,   this, &cOusterStatusView::onImuIntrinsicsUpdated);
	connect(mpModel, &cOusterModel::updateLidarIntrinsics, this, &cOusterStatusView::onLidarIntrinsicsUpdated);
	connect(mpModel, &cOusterModel::updateDataFormat,      this, &cOusterStatusView::onDataFormatUpdated);
	connect(mpModel, &cOusterModel::updateAzimuthWindow,   this, &cOusterStatusView::onAzimuthWindowUpdated);
	connect(mpModel, &cOusterModel::updateRangeData,       this, &cOusterStatusView::onRangeUpdated);

	cSensorStatusView::connectToModel();
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

void cOusterStatusView::onRangeUpdated(int range_mm)
{
	mpRange_mm->setText(QString::number(range_mm));
}

