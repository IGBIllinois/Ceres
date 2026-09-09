
#include "CeresOptionsDlg.hpp"

#include "Sound.hpp"

#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QComboBox>


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

QString cCeresOptionsDlg::endOfExperimentWavFilename() const
{
	return mpEndOfExperimentWavFilename->text();
}

void cCeresOptionsDlg::setEndOfExperimentWavFilename(QString filename)
{
	mpEndOfExperimentWavFilename->setText(filename);
}

QString cCeresOptionsDlg::experimentErrorWavFilename() const
{
	return mpExperimentErrorWavFilename->text();
}

void cCeresOptionsDlg::setExperimentErrorWavFilename(QString filename)
{
	mpExperimentErrorWavFilename->setText(filename);
}

QString cCeresOptionsDlg::experimentAttentionWavFilename() const
{
	return mpExperimentAttentionWavFilename->text();
}

void cCeresOptionsDlg::setExperimentAttentionWavFilename(QString filename)
{
	mpExperimentAttentionWavFilename->setText(filename);
}

QString cCeresOptionsDlg::fieldLayoutFilename() const
{
	return mpFieldLayoutFilename->text();
}

void cCeresOptionsDlg::setFieldLayoutFilename(QString filename)
{
	mpFieldLayoutFilename->setText(filename);
}

QString cCeresOptionsDlg::experimentPath() const
{
	return mpDefaultExperimentPath->text();
}

void cCeresOptionsDlg::setExperimentPath(QString path)
{
	mpDefaultExperimentPath->setText(path);
}

void cCeresOptionsDlg::createControls()
{
	mpEndOfExperimentWavFilename = new QLineEdit(this);
	mpEndOfExperimentWavFilename->setMinimumWidth(300);
	mpBrowseEndOfExperimentWavFile = new QPushButton("Browse", this);
	connect(mpBrowseEndOfExperimentWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::browseEndOfExperimentWavFile);
	mpTestEndOfExperimentWavFile = new QPushButton("Test", this);
	connect(mpTestEndOfExperimentWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::testEndOfExperimentWavFiles);

	mpExperimentErrorWavFilename = new QLineEdit(this);
	mpExperimentErrorWavFilename->setMinimumWidth(300);
	mpBrowseExperimentErrorWavFile = new QPushButton("Browse", this);
	connect(mpBrowseExperimentErrorWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::browseExperimentErrorWavFile);
	mpTestExperimentErrorWavFile = new QPushButton("Test", this);
	connect(mpTestExperimentErrorWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::testExperimentErrorWavFiles);

	mpExperimentAttentionWavFilename = new QLineEdit(this);
	mpExperimentAttentionWavFilename->setMinimumWidth(300);
	mpBrowseExperimentAttentionWavFile = new QPushButton("Browse", this);
	connect(mpBrowseExperimentAttentionWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::browseExperimentAttentionWavFile);
	mpTestExperimentAttentionWavFile = new QPushButton("Test", this);
	connect(mpTestExperimentAttentionWavFile, &QPushButton::pressed, this, &cCeresOptionsDlg::testExperimentAttentionWavFiles);


	mpFieldLayoutFilename = new QLineEdit(this);
	mpBrowseFieldLayout = new QPushButton("Browse", this);
	connect(mpBrowseFieldLayout, &QPushButton::pressed, this, &cCeresOptionsDlg::browseFieldLayout);

	mpDefaultExperimentPath = new QLineEdit(this);
	mpBrowseExperimentPath = new QPushButton("Browse", this);
	connect(mpBrowseExperimentPath, &QPushButton::pressed, this, &cCeresOptionsDlg::browseExperimentPath);

	mpEndOfExperimentAudioID = new QComboBox(this);
	mpExperimentErrorAudioID = new QComboBox(this);
	mpExperimentAttentionAudioID = new QComboBox(this);

	cAudioDevices audio_devices;
	auto devices = audio_devices.getAudioDevices();

	for (const auto& device : devices)
	{
		mpEndOfExperimentAudioID->addItem(QString::fromStdString(device.audio_device_name), device.audio_device_ID);
		mpExperimentErrorAudioID->addItem(QString::fromStdString(device.audio_device_name), device.audio_device_ID);
		mpExperimentAttentionAudioID->addItem(QString::fromStdString(device.audio_device_name), device.audio_device_ID);
	}
}

void cCeresOptionsDlg::createLayout()
{
	QLabel* text = nullptr;
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGroupBox* pGroupBox = new QGroupBox(tr("End of Experiment"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QVBoxLayout* pWavLayout = new QVBoxLayout();

	QHBoxLayout* pSoundLayout = new QHBoxLayout();

	text = new QLabel("WAV File : ");
	pSoundLayout->addWidget(text);
	pSoundLayout->addWidget(mpEndOfExperimentWavFilename);
	pSoundLayout->addWidget(mpBrowseEndOfExperimentWavFile);
	pSoundLayout->addWidget(mpTestEndOfExperimentWavFile);

	pWavLayout->addLayout(pSoundLayout);

	QHBoxLayout* pOutputLayout = new QHBoxLayout();

	text = new QLabel("Audio Device : ");
	pOutputLayout->addWidget(text);
	pOutputLayout->addWidget(mpEndOfExperimentAudioID);

	pWavLayout->addLayout(pOutputLayout);

	pGroupBox->setLayout(pWavLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(5);

	pGroupBox = new QGroupBox(tr("Experiment Error"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pWavLayout = new QVBoxLayout();

	pSoundLayout = new QHBoxLayout();

	text = new QLabel("WAV File : ");
	pSoundLayout->addWidget(text);
	pSoundLayout->addWidget(mpExperimentErrorWavFilename);
	pSoundLayout->addWidget(mpBrowseExperimentErrorWavFile);
	pSoundLayout->addWidget(mpTestExperimentErrorWavFile);

	pWavLayout->addLayout(pSoundLayout);

	pOutputLayout = new QHBoxLayout();

	text = new QLabel("Audio Device : ");
	pOutputLayout->addWidget(text);
	pOutputLayout->addWidget(mpExperimentErrorAudioID);

	pWavLayout->addLayout(pOutputLayout);

	pGroupBox->setLayout(pWavLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(5);

	pGroupBox = new QGroupBox(tr("Experiment Error"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pWavLayout = new QVBoxLayout();

	pSoundLayout = new QHBoxLayout();

	text = new QLabel("WAV File : ");
	pSoundLayout->addWidget(text);
	pSoundLayout->addWidget(mpExperimentAttentionWavFilename);
	pSoundLayout->addWidget(mpBrowseExperimentAttentionWavFile);
	pSoundLayout->addWidget(mpTestExperimentAttentionWavFile);

	pWavLayout->addLayout(pSoundLayout);

	pOutputLayout = new QHBoxLayout();

	text = new QLabel("Audio Device : ");
	pOutputLayout->addWidget(text);
	pOutputLayout->addWidget(mpExperimentAttentionAudioID);

	pWavLayout->addLayout(pOutputLayout);

	pGroupBox->setLayout(pWavLayout);
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

void cCeresOptionsDlg::browseEndOfExperimentWavFile()
{
	QString defaultDirectory = mpEndOfExperimentWavFilename->text();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load WAV file"), defaultDirectory,
		"Wav Files (*.wav)");

	if (fileName.isEmpty())
		return;

	mpEndOfExperimentWavFilename->setText(fileName);
}

void cCeresOptionsDlg::browseExperimentErrorWavFile()
{
	QString defaultDirectory = mpExperimentErrorWavFilename->text();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load WAV file"), defaultDirectory,
		"Wav Files (*.wav)");

	if (fileName.isEmpty())
		return;

	mpExperimentErrorWavFilename->setText(fileName);
}

void cCeresOptionsDlg::browseExperimentAttentionWavFile()
{
	QString defaultDirectory = mpExperimentAttentionWavFilename->text();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Load WAV file"), defaultDirectory,
		"Wav Files (*.wav)");

	if (fileName.isEmpty())
		return;

	mpExperimentAttentionWavFilename->setText(fileName);
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

void cCeresOptionsDlg::testEndOfExperimentWavFiles()
{
	std::string wavFilename = mpEndOfExperimentWavFilename->text().toStdString();

	if (wavFilename.empty())
		return;

	SDL_AudioDeviceID id = mpEndOfExperimentAudioID->currentData().toInt();

	if (mTestSound.is_open())
		mTestSound.close();

	mTestSound.setPlaybackDevice(id);
	if (mTestSound.open(wavFilename))
		mTestSound.play();
}

void cCeresOptionsDlg::testExperimentErrorWavFiles()
{
	std::string wavFilename = mpExperimentErrorWavFilename->text().toStdString();

	if (wavFilename.empty())
		return;

	SDL_AudioDeviceID id = mpExperimentErrorAudioID->currentData().toInt();

	if (mTestSound.is_open())
		mTestSound.close();

	mTestSound.setPlaybackDevice(id);
	if (mTestSound.open(wavFilename))
		mTestSound.play();
}

void cCeresOptionsDlg::testExperimentAttentionWavFiles()
{
	std::string wavFilename = mpExperimentAttentionWavFilename->text().toStdString();

	if (wavFilename.empty())
		return;

	SDL_AudioDeviceID id = mpExperimentAttentionAudioID->currentData().toInt();

	if (mTestSound.is_open())
		mTestSound.close();

	mTestSound.setPlaybackDevice(id);
	if (mTestSound.open(wavFilename))
		mTestSound.play();
}
