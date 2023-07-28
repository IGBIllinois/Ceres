
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
	cHySpexCamera_StatusView(pModel, parent)
{
}

cHySpexVNIR_3000N_StatusView::~cHySpexVNIR_3000N_StatusView()
{
}

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


