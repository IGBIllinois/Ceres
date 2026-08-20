
#include "HySpexVNIR_3000N_StatusView.hpp"
#include "HySpexVNIR_3000N_Model.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <QCustomPlot/qcustomplot.h>

#include <string>


cHySpexVNIR_3000N_StatusView::cHySpexVNIR_3000N_StatusView(cHySpexVNIR_3000N_Model* pModel, QWidget* parent)
:
	cHySpexCamera_StatusView(pModel, parent), mpModel(pModel)
{
	assert(mpModel);
}

cHySpexVNIR_3000N_StatusView::~cHySpexVNIR_3000N_StatusView()
{}

void cHySpexVNIR_3000N_StatusView::createWidgets()
{
	cHySpexCamera_StatusView::createWidgets();
}

void cHySpexVNIR_3000N_StatusView::doLayout()
{
	qInfo() << "Delete old layout...";

	auto old_layout = layout();
	delete old_layout;

	auto* mainLayout = new QVBoxLayout(this);

	qInfo() << "Calling getSensorStatusBox...";
	mainLayout->addWidget(getSensorStatusBox());

	qInfo() << "Calling doStatusLayout...";
	doStatusLayout(mainLayout);

	auto* infoLayout = new QHBoxLayout(this);

	qInfo() << "Calling doAcqStatusLayout...";
	doAcqStatusLayout(infoLayout);

	qInfo() << "Calling doLensInfoLayout...";
	doLensInfoLayout(infoLayout);

	mainLayout->addLayout(infoLayout);

	qInfo() << "Calling doPlotLayout...";
	doPlotLayout(mainLayout);

//	mainLayout->addStretch();

	qInfo() << "Setting main layout...";
	setLayout(mainLayout);
}

void cHySpexVNIR_3000N_StatusView::connectToModel()
{
	cHySpexCamera_StatusView::connectToModel();

	connect(mpModel, &cHySpexVNIR_3000N_Model::initStatusChanged,    this, &cHySpexVNIR_3000N_StatusView::onInitStatusChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::commStatusChanged,    this, &cHySpexVNIR_3000N_StatusView::onCommStatusChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::acqStatusChanged,     this, &cHySpexVNIR_3000N_StatusView::onAcqStatusChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::bgStatusChanged,      this, &cHySpexVNIR_3000N_StatusView::onBgStatusChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::coolingStatusChanged, this, &cHySpexVNIR_3000N_StatusView::onCoolingStatusChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::shutterStatusChanged, this, &cHySpexVNIR_3000N_StatusView::onShutterStatusChange);

	connect(mpModel, &cHySpexVNIR_3000N_Model::lensInfoChanged, this, &cHySpexVNIR_3000N_StatusView::onLensInfoChange);

	connect(mpModel, &cHySpexVNIR_3000N_Model::avgFramesChanged,          this, &cHySpexVNIR_3000N_StatusView::onAvgFramesChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::framePeriodChanged,        this, &cHySpexVNIR_3000N_StatusView::onFramePeriodChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::minFramePeriodChanged,     this, &cHySpexVNIR_3000N_StatusView::onMinFramePeriodChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::integrationTimeChanged,    this, &cHySpexVNIR_3000N_StatusView::onIntegrationTimeChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::maxIntegrationTimeChanged, this, &cHySpexVNIR_3000N_StatusView::onMaxIntegrationTimeChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::ambientTempChanged,        this, &cHySpexVNIR_3000N_StatusView::onAmbientTempChange);
	connect(mpModel, &cHySpexVNIR_3000N_Model::sensorTempChanged,         this, &cHySpexVNIR_3000N_StatusView::onSensorTempChange);

	connect(mpModel, &cHySpexVNIR_3000N_Model::computeModeChanged, this, &cHySpexVNIR_3000N_StatusView::onComputeModeChange);

	connect(mpModel, &cHySpexVNIR_3000N_Model::newPercentSaturationData,    this, &cHySpexVNIR_3000N_StatusView::onSaturationDataUpdated);
	connect(mpModel, &cHySpexVNIR_3000N_Model::newPercentBandData,          this, &cHySpexVNIR_3000N_StatusView::onBandDataUpdated);
	connect(mpModel, &cHySpexVNIR_3000N_Model::newFocusData,                this, &cHySpexVNIR_3000N_StatusView::onFocusDataUpdated);
	connect(mpModel, &cHySpexVNIR_3000N_Model::newSpatialDistributionData,  this, &cHySpexVNIR_3000N_StatusView::onSpatialDistributionUpdated);
	connect(mpModel, &cHySpexVNIR_3000N_Model::newSpectralDistributionData, this, &cHySpexVNIR_3000N_StatusView::onSpectralDistributionUpdated);
}

