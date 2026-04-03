

#include "LucidVisionLabsRgbPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>


cLucidVisionLabsRgbPropertyPage::cLucidVisionLabsRgbPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{
}

void cLucidVisionLabsRgbPropertyPage::createWidgets()
{
    QLabel* pLabel = nullptr;

    mpModeLabel = new QLabel("Mode:", this);
    mpMode = new QComboBox(this);
    mpMode->addItem("Photo");
    mpMode->addItem("Time Lapse");
    mpMode->addItem("Video");
    connect(mpMode, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::modeTextChanged);

    mpFrameRateLabel = new QLabel("Frames per Second:", this);
    mpFrameRate_fps = new QLineEdit(this);

    mpLapseIntervalLabel = new QLabel("Time-Lapse Interval (s):", this);;
    mpLapseInterval_s = new QLineEdit(this);

    mpGrabImage = new QPushButton("Grab Image", this);
    connect(mpGrabImage, &QPushButton::pressed, this, &cLucidVisionLabsRgbPropertyPage::onGrabImagePressed);
    mpGrabImage->setEnabled(false);

    mpAcqTime_Hz = new QLineEdit();
    mpAcqTime_Hz->setEnabled(false);

    mpExposureTimeSelector = new QComboBox();
    mpExposureTimeSelector->addItem("Common");
    mpExposureTimeSelector->setEnabled(false);
    connect(mpExposureTimeSelector, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::exposureTimeSelectorChanged);

    mpExposureTime_us = new QLineEdit();
    mpExposureTime_us->setEnabled(false);

    mpExposureAutoMode = new QComboBox();
    mpExposureAutoMode->addItem("Off");
    mpExposureAutoMode->addItem("Once");
    mpExposureAutoMode->addItem("Continuous");
    mpExposureAutoMode->setEnabled(false);
    connect(mpExposureAutoMode, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::exposureAutoModeChanged);

    mpExposureInfo = new QGroupBox("Exposure Info");
    auto* expInfoLayout = new QFormLayout();

    pLabel = new QLabel("Acquisition Frame Rate (Hz)");
    expInfoLayout->addRow(pLabel, mpAcqTime_Hz);

    pLabel = new QLabel("Exposure Time Selector");
    expInfoLayout->addRow(pLabel, mpExposureTimeSelector);

    pLabel = new QLabel("Exposure Time (us)");
    expInfoLayout->addRow(pLabel, mpExposureTime_us);

    pLabel = new QLabel("Exposure Auto Mode");
    expInfoLayout->addRow(pLabel, mpExposureAutoMode);
    mpExposureInfo->setLayout(expInfoLayout);

    mpPixelFormat = new QComboBox(this);
    mpPixelFormat->addItem("Mono8");
    mpPixelFormat->addItem("Mono10");
    mpPixelFormat->addItem("Mono10p");
    mpPixelFormat->addItem("Mono10Packed");
    mpPixelFormat->addItem("Mono12");
    mpPixelFormat->addItem("Mono12p");
    mpPixelFormat->addItem("Mono12Packed");
    mpPixelFormat->addItem("Mono16");
    mpPixelFormat->addItem("BayerRG8");
    mpPixelFormat->addItem("BayerRG10");
    mpPixelFormat->addItem("BayerRG10p");
    mpPixelFormat->addItem("BayerRG10Packed");
    mpPixelFormat->addItem("BayerRG12");
    mpPixelFormat->addItem("BayerRG12p");
    mpPixelFormat->addItem("BayerRG12Packed");
    mpPixelFormat->addItem("BayerRG16");
    mpPixelFormat->addItem("RGB8");
    mpPixelFormat->addItem("BGR8");
    mpPixelFormat->addItem("YCbCr8");
    mpPixelFormat->addItem("YCbCr8_CbYCr");
    mpPixelFormat->addItem("YUV422_8");
    mpPixelFormat->addItem("YUV422_8_UYVY");
    mpPixelFormat->addItem("YCbCr411_8");
    mpPixelFormat->addItem("YUV411_8_UYYVYY");
    mpPixelFormat->addItem("QOI_Mono8");
    mpPixelFormat->addItem("QOI_BayerRG8");
    mpPixelFormat->addItem("QOI_RGB8");
    mpPixelFormat->addItem("QOI_BGR8");
    mpPixelFormat->addItem("QOI_YCbCr8");
    mpPixelFormat->addItem("QOI_YCbCr8_CbYC");
    mpPixelFormat->setEnabled(false);
    connect(mpPixelFormat, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::pixelFormatChanged);

    mpImageSize = new QLineEdit(this);
    mpImageSize->setReadOnly(true);

    mpPixelFormatInfo = new QGroupBox("Pixel Format Info");
    auto* pixelInfoLayout = new QFormLayout();

    pLabel = new QLabel("Pixel Format");
    pixelInfoLayout->addRow(pLabel, mpPixelFormat);

    pLabel = new QLabel("Image Size");
    pixelInfoLayout->addRow(pLabel, mpImageSize);

    mpPixelFormatInfo->setLayout(pixelInfoLayout);


    mpGain_dB = new QLineEdit(this);
    mpGain_dB->setEnabled(false);

    mpGainAutoMode = new QComboBox(this);
    mpGainAutoMode->addItem("Off");
    mpGainAutoMode->addItem("Once");
    mpGainAutoMode->addItem("Continuous");
    mpGainAutoMode->setEnabled(false);
    connect(mpGainAutoMode, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::gainAutoModeChanged);

    mpBalanceWhiteAutoMode = new QComboBox(this);
    mpBalanceWhiteAutoMode->addItem("Off");
    mpBalanceWhiteAutoMode->addItem("Once");
    mpBalanceWhiteAutoMode->addItem("Continuous");
    mpBalanceWhiteAutoMode->setEnabled(false);
    connect(mpBalanceWhiteAutoMode, &QComboBox::currentTextChanged, this, &cLucidVisionLabsRgbPropertyPage::balanceWhiteAutoModeChanged);

    mpGammaEnable = new QCheckBox(this);
    mpGammaEnable->setEnabled(false);
    connect(mpGammaEnable, &QCheckBox::clicked, this, &cLucidVisionLabsRgbPropertyPage::gammaEnableChanged);

    mpGamma = new QLineEdit(this);
    mpGamma->setEnabled(false);


    mpAnalogCtrlInfo = new QGroupBox("Analog Control");
    auto* gainInfoLayout = new QFormLayout();

    pLabel = new QLabel("Gain (dB)");
    gainInfoLayout->addRow(pLabel, mpGain_dB);

    pLabel = new QLabel("Gain Auto Mode");
    gainInfoLayout->addRow(pLabel, mpGainAutoMode);

    pLabel = new QLabel("Balance White Auto");
    gainInfoLayout->addRow(pLabel, mpBalanceWhiteAutoMode);

    pLabel = new QLabel("Gamma Enable");
    gainInfoLayout->addRow(pLabel, mpGammaEnable);

    pLabel = new QLabel("Gamma");
    gainInfoLayout->addRow(pLabel, mpGamma);
    mpAnalogCtrlInfo->setLayout(gainInfoLayout);
}

void cLucidVisionLabsRgbPropertyPage::doLayout()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();
//    pMainLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto* pLayout = new QFormLayout();

    pLayout->addRow(mpModeLabel, mpMode);
    pLayout->addRow(mpFrameRateLabel, mpFrameRate_fps);
    pLayout->addRow(mpLapseIntervalLabel, mpLapseInterval_s);
    pMainLayout->addLayout(pLayout);

    pMainLayout->addSpacing(5);

    pMainLayout->addWidget(mpExposureInfo);

    pMainLayout->addSpacing(5);

    pMainLayout->addWidget(mpPixelFormatInfo);

    pMainLayout->addSpacing(5);

    pMainLayout->addWidget(mpAnalogCtrlInfo);

    pMainLayout->addSpacing(5);

    pMainLayout->addWidget(mpGrabImage, 1);

    pMainLayout->addSpacing(10);

    pMainLayout->addWidget(mpButtons);

    setLayout(pMainLayout);
}

void cLucidVisionLabsRgbPropertyPage::enableControls(bool enable)
{
    mpAcqTime_Hz->setEnabled(enable);
    mpExposureTimeSelector->setEnabled(enable);
    mpExposureTime_us->setEnabled(enable);
    mpExposureAutoMode->setEnabled(enable);

    mpPixelFormat->setEnabled(enable);

    mpGain_dB->setEnabled(enable);
    mpGainAutoMode->setEnabled(enable);
    mpBalanceWhiteAutoMode->setEnabled(enable);
    mpGammaEnable->setEnabled(enable);
    mpGamma->setEnabled(enable);

    mpGrabImage->setEnabled(enable);
}

cExperimentState* cLucidVisionLabsRgbPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}

void cLucidVisionLabsRgbPropertyPage::modeTextChanged(const QString& text)
{
    if (text == "Photo")
    {
        mpFrameRate_fps->setEnabled(false);
        mpLapseInterval_s->setEnabled(false);
    }
    else if (text == "Time Lapse")
    {
        mpFrameRate_fps->setEnabled(false);
        mpLapseInterval_s->setEnabled(true);
    }
    else if (text == "Video")
    {
        mpLapseInterval_s->setEnabled(false);
        mpFrameRate_fps->setEnabled(true);
    }
}
