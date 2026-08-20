
#include "HySpexSWIR_384_StatusView.hpp"
#include "HySpexSWIR_384_Model.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <QCustomPlot/qcustomplot.h>

#include <string>


cHySpexSWIR_384_StatusView::cHySpexSWIR_384_StatusView(cHySpexSWIR_384_Model* pModel, QWidget* parent)
:
	cHySpexCamera_StatusView(pModel, parent), mpModel(pModel)
{
	assert(mpModel);
}

cHySpexSWIR_384_StatusView::~cHySpexSWIR_384_StatusView()
{
}

void cHySpexSWIR_384_StatusView::createWidgets()
{
	cHySpexCamera_StatusView::createWidgets();
}

void cHySpexSWIR_384_StatusView::doLayout()
{
	auto old_layout = layout();
	if (old_layout)
	{
		qInfo() << "Delete old layout...";

		old_layout->deleteLater();
	}

	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	doStatusLayout(mainLayout);

	auto* infoLayout = new QHBoxLayout();

	doAcqStatusLayout(infoLayout);
	doLensInfoLayout(infoLayout);

	mainLayout->addLayout(infoLayout);

	doPlotLayout(mainLayout);

	setLayout(mainLayout);
}

void cHySpexSWIR_384_StatusView::connectToModel()
{
	cHySpexCamera_StatusView::connectToModel();

	connect(mpModel, &cHySpexSWIR_384_Model::initStatusChanged,    this, &cHySpexSWIR_384_StatusView::onInitStatusChange);
	connect(mpModel, &cHySpexSWIR_384_Model::commStatusChanged,    this, &cHySpexSWIR_384_StatusView::onCommStatusChange);
	connect(mpModel, &cHySpexSWIR_384_Model::acqStatusChanged,     this, &cHySpexSWIR_384_StatusView::onAcqStatusChange);
	connect(mpModel, &cHySpexSWIR_384_Model::bgStatusChanged,      this, &cHySpexSWIR_384_StatusView::onBgStatusChange);
	connect(mpModel, &cHySpexSWIR_384_Model::coolingStatusChanged, this, &cHySpexSWIR_384_StatusView::onCoolingStatusChange);
	connect(mpModel, &cHySpexSWIR_384_Model::shutterStatusChanged, this, &cHySpexSWIR_384_StatusView::onShutterStatusChange);

	connect(mpModel, &cHySpexSWIR_384_Model::lensInfoChanged, this, &cHySpexSWIR_384_StatusView::onLensInfoChange);

	connect(mpModel, &cHySpexSWIR_384_Model::avgFramesChanged,          this, &cHySpexSWIR_384_StatusView::onAvgFramesChange);
	connect(mpModel, &cHySpexSWIR_384_Model::framePeriodChanged,        this, &cHySpexSWIR_384_StatusView::onFramePeriodChange);
	connect(mpModel, &cHySpexSWIR_384_Model::minFramePeriodChanged,     this, &cHySpexSWIR_384_StatusView::onMinFramePeriodChange);
	connect(mpModel, &cHySpexSWIR_384_Model::integrationTimeChanged,    this, &cHySpexSWIR_384_StatusView::onIntegrationTimeChange);
	connect(mpModel, &cHySpexSWIR_384_Model::maxIntegrationTimeChanged, this, &cHySpexSWIR_384_StatusView::onMaxIntegrationTimeChange);
	connect(mpModel, &cHySpexSWIR_384_Model::ambientTempChanged,        this, &cHySpexSWIR_384_StatusView::onAmbientTempChange);
	connect(mpModel, &cHySpexSWIR_384_Model::sensorTempChanged,         this, &cHySpexSWIR_384_StatusView::onSensorTempChange);

	connect(mpModel, &cHySpexSWIR_384_Model::computeModeChanged, this, &cHySpexSWIR_384_StatusView::onComputeModeChange);

	connect(mpModel, &cHySpexSWIR_384_Model::newPercentSaturationData,    this, &cHySpexSWIR_384_StatusView::onSaturationDataUpdated);
	connect(mpModel, &cHySpexSWIR_384_Model::newPercentBandData,          this, &cHySpexSWIR_384_StatusView::onBandDataUpdated);
	connect(mpModel, &cHySpexSWIR_384_Model::newFocusData,                this, &cHySpexSWIR_384_StatusView::onFocusDataUpdated);
	connect(mpModel, &cHySpexSWIR_384_Model::newSpatialDistributionData,  this, &cHySpexSWIR_384_StatusView::onSpatialDistributionUpdated);
	connect(mpModel, &cHySpexSWIR_384_Model::newSpectralDistributionData, this, &cHySpexSWIR_384_StatusView::onSpectralDistributionUpdated);
}

void cHySpexSWIR_384_StatusView::onSensorTempChange(double temp_C)
{
	double temp_K = 273.15 + temp_C;
	QString label = QString::number(temp_K, 'f', 1);
	label += " K";
	mpCoolingStatus->setText(label);
}



