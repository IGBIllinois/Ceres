
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
	cHySpexStatusView(pModel, parent)
{
}

cHySpexVNIR_3000N_StatusView::~cHySpexVNIR_3000N_StatusView()
{
}

void cHySpexVNIR_3000N_StatusView::createWidgets()
{
	cHySpexStatusView::createWidgets();
}

void cHySpexVNIR_3000N_StatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	doStatusLayout(mainLayout);

	auto* infoLayout = new QHBoxLayout(this);

	doAcqStatusLayout(infoLayout);
	doLensInfoLayout(infoLayout);

	mainLayout->addLayout(infoLayout);

	doPlotLayout(mainLayout);

//	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cHySpexVNIR_3000N_StatusView::resizeEvent(QResizeEvent* event)
{
//	auto layout = QWidget::layout();
//	auto layout_rect = layout->contentsRect();

//	auto win_size = event->size();
//	mpPlot->setMinimumHeight(400);

	QWidget::resizeEvent(event);
}
