
#include "FlirConfigureDlg.hpp"

#include "Constants.hpp"

#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include <algorithm>


cFlirConfigureDlg::cFlirConfigureDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("FLIR Configuration");

	createControls();
	createLayout();
}

cFlirConfigureDlg::~cFlirConfigureDlg()
{}

int cFlirConfigureDlg::mode() const
{
	return mpMode->currentIndex();
}

int cFlirConfigureDlg::lapseInterval_ms() const
{
	return static_cast<int>(mpLapseInterval_s->text().toDouble()* nConstants::SEC_TO_MS);
}

double cFlirConfigureDlg::frameRate_fps() const
{
	return mpFrameRate_fps->text().toDouble();
}

void cFlirConfigureDlg::setMode(int mode)
{
	mpMode->setCurrentIndex(mode);
}

void cFlirConfigureDlg::setLapseInterval_ms(int interval_ms)
{
	mpLapseInterval_s->setText(QString::number(interval_ms * nConstants::MS_TO_SEC, 103, 3));
}

void cFlirConfigureDlg::setFrameRate_fps(double frame_rate_fps)
{
	mpFrameRate_fps->setText(QString::number(frame_rate_fps, 103, 3));
}

void cFlirConfigureDlg::createControls()
{
	mpModeLabel = new QLabel("Mode:", this);
	mpMode = new QComboBox(this);
	mpMode->addItem("Photo");
	mpMode->addItem("Time Lapse");
	mpMode->addItem("Video");
	connect(mpMode, &QComboBox::currentIndexChanged, this, &cFlirConfigureDlg::modeChanged);

/*
	mpImageSizeLabel = new QLabel("Image Size (w x d):", this);
	mpImageSize = new QLineEdit(this);
	mpImageSize->setReadOnly(true);
*/

	mpFrameRateLabel = new QLabel("Frames per Second:", this);
	mpFrameRate_fps = new QLineEdit(this);

	mpLapseIntervalLabel = new QLabel("Time-Lapse Interval (s):", this);;
	mpLapseInterval_s = new QLineEdit(this);

/*
	mpThermalRangeLabel = new QLabel("Thermal Range (K):", this);
	mpThermalRange = new QLineEdit(this);
	mpThermalRange->setReadOnly(true);
*/

	mpLapseInterval_s->setValidator(new QDoubleValidator(0.1, 3600.0, 1));


	mpButtons = new QDialogButtonBox(QDialogButtonBox::Ok |
		QDialogButtonBox::Cancel, Qt::Horizontal, this);

	connect(mpButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(mpButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void cFlirConfigureDlg::createLayout()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	auto* pLayout = new QFormLayout();

	pLayout->addRow(mpModeLabel, mpMode);
//*	pLayout->addRow(mpImageSizeLabel, mpImageSize);
	pLayout->addRow(mpFrameRateLabel, mpFrameRate_fps);
	pLayout->addRow(mpLapseIntervalLabel, mpLapseInterval_s);
//*	pLayout->addRow(mpThermalRangeLabel, mpThermalRange);
	pMainLayout->addLayout(pLayout);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

void cFlirConfigureDlg::modeChanged(int index)
{
	switch (index)
	{
	case 0:
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(false);
		break;
	case 1:
		mpFrameRate_fps->setEnabled(false);
		mpLapseInterval_s->setEnabled(true);
		break;
	case 2:
		mpLapseInterval_s->setEnabled(false);
		mpFrameRate_fps->setEnabled(true);
		break;
	default:
		return;
	}
}

