
#include "ExperimentFieldLayoutDlg.hpp"
#include "FieldLayoutDlg.hpp"
#include "ExperimentSteps.hpp"
#include "Constants.hpp"

#include <QLabel>
#include <QComboBox>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>


cExperimentFieldLayoutDlg::cExperimentFieldLayoutDlg(cFieldLayoutWidget& fieldWidget, QWidget* parent)
:
	mFieldWidget(fieldWidget), QDialog(parent)
{
	setWindowTitle("Experiment Layout");

	createControls();
	createLayout();
}

cExperimentFieldLayoutDlg::~cExperimentFieldLayoutDlg()
{}


const cSpidercamScanArea::experimentLayout_t& cExperimentFieldLayoutDlg::getLayout() const
{
	return mLayout;
}

const cSpidercamScanArea::experimentLayout_t& cExperimentFieldLayoutDlg::getOriginalLayout() const
{
	return mOriginalLayout;
}

void cExperimentFieldLayoutDlg::setExperiment(const cExperimentFile& experiment)
{
	bool hasStartingPoint = false;

	int x1_mm = 0;
	int y1_mm = 0;

	for (auto step : experiment)
	{
		auto movement = dynamic_cast<cExperimentStep_Movement*>(step.get());

		if (movement)
		{
			if (movement->getX_mm().has_value() && movement->getY_mm().has_value())
			{
				if (movement->isRecording())
				{
					mEndX_m = movement->getX_mm().value() * nConstants::MM_TO_M;
					mEndY_m = movement->getY_mm().value() * nConstants::MM_TO_M;

					if (!hasStartingPoint)
					{
						mStartX_m = x1_mm * nConstants::MM_TO_M;
						mStartY_m = y1_mm * nConstants::MM_TO_M;
						hasStartingPoint = true;
					}
				}
				else
				{
					x1_mm = movement->getX_mm().value();
					y1_mm = movement->getY_mm().value();
				}
			}
		}
	}

	if (mEndX_m < mStartX_m)
		std::swap(mStartX_m, mEndX_m);

	if (mEndY_m < mStartY_m)
		std::swap(mStartY_m, mEndY_m);

	mName = QString::fromStdString(experiment.getLayoutName());
	QString label = mName;

	if (mName.isEmpty())
	{
		mName = QString::fromStdString(experiment.getExperimentName());
		label = mName.left(mName.indexOf(' '));
	}

	int item = mpPlotLayouts->findText(label, Qt::MatchStartsWith);

	if (item >= 0)
	{
		for (const auto& entry : mFieldWidget.getLayouts())
		{
			if (entry.caption.label.startsWith(label))
			{
				mName = entry.caption.label;
				mpPlotLayouts->setCurrentIndex(item);
				mOriginalLayout = entry;
				break;
			}
		}
	}
}

void cExperimentFieldLayoutDlg::onNewLayout()
{
	cFieldLayoutDlg dlg(this);

	QColor color;
	color.setRed(255);
	color.setGreen(0);
	color.setBlue(0);
	color.setAlpha(128);

	float northPadding_m = mpNorthPadding_m->text().toFloat();
	float southPadding_m = mpSouthPadding_m->text().toFloat();
	float eastPadding_m = mpEastPadding_m->text().toFloat();
	float westPadding_m = mpWestPadding_m->text().toFloat();

	cSpidercamScanArea::experimentLayout_t layout;
	layout.caption.label = mName;
	layout.caption.font_size = 12;
	layout.caption.orientation_deg = 0.0;
	layout.caption.horizontal_align = cSpidercamScanArea::eHorizontalAlignment::CENTER;
	layout.caption.vertical_align   = cSpidercamScanArea::eVerticalAlignment::CENTER;
	layout.caption.color = color;

	layout.color = color;

	layout.north_m = mStartX_m - northPadding_m;
	layout.south_m = mEndX_m + southPadding_m;
	layout.east_m = mEndY_m + eastPadding_m;
	layout.west_m = mStartY_m - westPadding_m;

	dlg.setDefaults(layout);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	layout = dlg.getLayout();

	mFieldWidget.addLayout(layout);

	mpPlotLayouts->clear();
	for (const auto& entry : mFieldWidget.getLayouts())
	{
		mpPlotLayouts->addItem(entry.caption.label);
	}

	mpPlotLayouts->setCurrentText(layout.caption.label);
	mOriginalLayout = layout;
}

void cExperimentFieldLayoutDlg::onLayoutChange(const QString& text)
{
	auto layout = mFieldWidget.findLayout(text);

	if (layout)
	{
		mOriginalLayout = layout;
	}
}

void cExperimentFieldLayoutDlg::accept()
{
	if (mOriginalLayout.caption.label.isEmpty())
	{
		QString label = mpPlotLayouts->currentText();

		for (const auto& entry : mFieldWidget.getLayouts())
		{
			if (entry == label)
			{
				mOriginalLayout = entry;
				break;
			}
		}
	}

	mLayout = mOriginalLayout;

	float northPadding_m = mpNorthPadding_m->text().toFloat();
	float southPadding_m = mpSouthPadding_m->text().toFloat();
	float eastPadding_m = mpEastPadding_m->text().toFloat();
	float westPadding_m = mpWestPadding_m->text().toFloat();

	float north_m = mStartX_m - northPadding_m;
	float south_m = mEndX_m + southPadding_m;
	float east_m = mEndY_m + eastPadding_m;
	float west_m = mStartY_m - westPadding_m;

	if (north_m < mLayout.north_m) mLayout.north_m = north_m;
	if (south_m > mLayout.south_m) mLayout.south_m = south_m;

	if (west_m < mLayout.west_m) mLayout.west_m = west_m;
	if (east_m > mLayout.east_m) mLayout.east_m = east_m;

	mLayout.computeBounds();

	QDialog::accept();
}

void cExperimentFieldLayoutDlg::createControls()
{
	mpPlotLayouts = new QComboBox(this);
	connect(mpPlotLayouts, &QComboBox::currentTextChanged, this, &cExperimentFieldLayoutDlg::onLayoutChange);

	for (const auto& entry : mFieldWidget.getLayouts())
	{
		mpPlotLayouts->addItem(entry.caption.label);
	}

	mpNewPlotLayout = new QPushButton("New", this);
	connect(mpNewPlotLayout, &QPushButton::pressed, this, &cExperimentFieldLayoutDlg::onNewLayout);

	mpNorthPadding_m = new QLineEdit(this);
	mpNorthPadding_m->setValidator(new QDoubleValidator(0, 10.0, 3));
	mpNorthPadding_m->setText("2.0");

	mpSouthPadding_m = new QLineEdit(this);
	mpSouthPadding_m->setValidator(new QDoubleValidator(0, 10.0, 3));
	mpSouthPadding_m->setText("2.0");

	mpEastPadding_m = new QLineEdit(this);
	mpEastPadding_m->setValidator(new QDoubleValidator(0, 10.0, 3));
	mpEastPadding_m->setText("2.0");

	mpWestPadding_m = new QLineEdit(this);
	mpWestPadding_m->setValidator(new QDoubleValidator(0, 10.0, 3));
	mpWestPadding_m->setText("2.0");
}

void cExperimentFieldLayoutDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pHLayout = new QHBoxLayout();

	pText = new QLabel("Layout");
	pHLayout->addWidget(pText);
	pHLayout->addWidget(mpPlotLayouts, 1);
	pHLayout->addWidget(mpNewPlotLayout);

	pMainLayout->addLayout(pHLayout);

	pGroupBox = new QGroupBox(tr("Bounding Box Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("North (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpNorthPadding_m, 0, 1);

	pText = new QLabel("South (m)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpSouthPadding_m, 0, 4);

	pText = new QLabel("West (m)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpWestPadding_m, 0, 7);

	pText = new QLabel("East (m)");
	pGridLayout->addWidget(pText, 0, 9);
	pGridLayout->addWidget(mpEastPadding_m, 0, 10);

	pGroupBox->setLayout(pGridLayout);

	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}



