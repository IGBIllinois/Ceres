
#include "CeresOptionsDlg.hpp"

#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QFileDialog>


cCeresOptionsDlg::cCeresOptionsDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Ceres Options");

	createControls();
	createLayout();
}

cCeresOptionsDlg::~cCeresOptionsDlg()
{}

QString cCeresOptionsDlg::wavFilename() const
{
	return mpWavFilename->text();
}

void cCeresOptionsDlg::setWavFilename(QString filename)
{
	mpWavFilename->setText(filename);
}

QString cCeresOptionsDlg::fieldLayoutFilename() const
{
	return mpFieldLayoutFilename->text();
}

void cCeresOptionsDlg::setFieldLayoutFilename(QString filename)
{
	mpWavFilename->setText(filename);
}

QString cCeresOptionsDlg::experimentPath() const
{
	return mpDefaultExperimentPath->text();
}

void cCeresOptionsDlg::setExperimentPath(QString path)
{
	mpWavFilename->setText(path);
}

void cCeresOptionsDlg::createControls()
{
	mpWavFilename = new QLineEdit(this);
	mpBrowseWavFile = new QPushButton("Browse", this);
	connect(mpBrowseWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::browseWavFiles);

	mpFieldLayoutFilename = new QLineEdit(this);
	mpBrowseFieldLayout = new QPushButton("Browse", this);
	connect(mpBrowseFieldLayout, &QPushButton::pressed, this, &cCeresOptionsDlg::browseFieldLayout);

	mpDefaultExperimentPath = new QLineEdit(this);
	mpBrowseExperimentPath = new QPushButton("Browse", this);
	connect(mpBrowseExperimentPath, &QPushButton::pressed, this, &cCeresOptionsDlg::browseExperimentPath);
}

void cCeresOptionsDlg::createLayout()
{
	QLabel* text = nullptr;
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGroupBox* pGroupBox = new QGroupBox(tr("End of Experiment"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pSoundLayout = new QHBoxLayout();

	text = new QLabel("WAV File : ");
	pSoundLayout->addWidget(text);
	pSoundLayout->addWidget(mpWavFilename);
	pSoundLayout->addWidget(mpBrowseWavFile);

	pGroupBox->setLayout(pSoundLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pFieldLayout = new QHBoxLayout();

	text = new QLabel("Field Layout File : ");
	pFieldLayout->addWidget(text);
	pFieldLayout->addWidget(mpFieldLayoutFilename);
	pFieldLayout->addWidget(mpBrowseFieldLayout);

	pMainLayout->addLayout(pFieldLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pExpLayout = new QHBoxLayout();

	text = new QLabel("Experiment File Path : ");
	pExpLayout->addWidget(text);
	pExpLayout->addWidget(mpDefaultExperimentPath);
	pExpLayout->addWidget(mpBrowseExperimentPath);

	pMainLayout->addLayout(pExpLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCeresOptionsDlg::browseWavFiles()
{
	QString defaultDirectory = mpWavFilename->text();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load WAV file"), defaultDirectory,
		"Wav Files (*.wav)");

	if (fileName.isEmpty())
		return;

	mpWavFilename->setText(fileName);
}

void cCeresOptionsDlg::browseFieldLayout()
{
	QString defaultDirectory = mpFieldLayoutFilename->text();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Field Layout file"), defaultDirectory,
		"Field Layout Files (*.json)");

	if (fileName.isEmpty())
		return;

	mpFieldLayoutFilename->setText(fileName);
}

void cCeresOptionsDlg::browseExperimentPath()
{

}
