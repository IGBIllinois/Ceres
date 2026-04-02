

#include "LucidVisionLabsRgbPropertyPage_Local.hpp"
#include "LucidVisionLabsRgbUtils.hpp"
#include "LucidVisionLabsRgbModel.hpp"
#include "LucidVisionLabsRgbModel_Triton.hpp"

#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>


cLucidVisionLabsRgbPropertyPage_Local::cLucidVisionLabsRgbPropertyPage_Local(cLucidVisionLabsRgbModel* pModel, QWidget* parent)
	: cLucidVisionLabsRgbPropertyPage(parent), mpModel(pModel)
{}

void cLucidVisionLabsRgbPropertyPage_Local::createWidgets()
{
	cLucidVisionLabsRgbPropertyPage::createWidgets();
}

void cLucidVisionLabsRgbPropertyPage_Local::doLayout()
{
	QString title = QString::fromStdString(mpModel->name());

	auto instance = mpModel->instance();

	if (!instance.empty())
	{
		title += ":";
		title += QString::fromStdString(instance);
	}

	setTitle(title);

	cLucidVisionLabsRgbPropertyPage::doLayout();
}

void cLucidVisionLabsRgbPropertyPage_Local::onGrabImagePressed()
{

}

void cLucidVisionLabsRgbPropertyPage_Local::exposureTimeSelectorChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::exposureAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::pixelFormatChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::gainAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::balanceWhiteAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::enableControls(bool enable)
{
	cLucidVisionLabsRgbPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cLucidVisionLabsRgbPropertyPage_Local::buttonClicked(QAbstractButton* button)
{
	cLucidVisionLabsRgbPropertyPage::buttonClicked(button);
}

void cLucidVisionLabsRgbPropertyPage_Local::showPage()
{
	switch (mpModel->mode())
	{
	case cLucidVisionLabsRgbModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cLucidVisionLabsRgbModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cLucidVisionLabsRgbModel::CONTINUOUS:
		mpMode->setCurrentIndex(2);
		break;
	}

	mpFrameRate_fps->setText(QString::number(mpModel->frameRate_Hz(), static_cast<char>(103), 4));

	double interval_s = mpModel->lapseInterval_ms() * 0.001;
	mpLapseInterval_s->setText(QString::number(interval_s, static_cast<char>(103), 4));

	QString image_size = QString::number(mpModel->imageWidth());
	image_size += " x ";
	image_size += QString::number(mpModel->imageHeight());

	mpImageSize->setText(image_size);

	mpGrabImage->setEnabled(true);

	cLucidVisionLabsRgbPropertyPage::showPage();
}

void cLucidVisionLabsRgbPropertyPage_Local::doOK()
{
	doApply();
	cLucidVisionLabsRgbPropertyPage::doOK();
}

void cLucidVisionLabsRgbPropertyPage_Local::doCancel()
{
	cLucidVisionLabsRgbPropertyPage::doCancel();
}

void cLucidVisionLabsRgbPropertyPage_Local::doApply()
{
	auto currentMode = mpModel->mode();

	if (currentMode != mpMode->currentIndex())
	{
		emit changeMode(mpMode->currentIndex());
	}

//	double frameRate_Hz() const;
//	uint32_t lapseInterval_ms() const;

}

void cLucidVisionLabsRgbPropertyPage_Local::reject()
{
	doCancel();
}


cLucidVisionLabsRgbPropertyPage_Local_Triton::cLucidVisionLabsRgbPropertyPage_Local_Triton(cLucidVisionLabsRgbModel_Triton* pModel, QWidget* parent)
	: cLucidVisionLabsRgbPropertyPage_Local(pModel, parent), mpModel(pModel)
{
}

void cLucidVisionLabsRgbPropertyPage_Local_Triton::showPage()
{
	cLucidVisionLabsRgbPropertyPage_Local::showPage();

	mpPixelFormat->setCurrentIndex(static_cast<int>(mpModel->pixelFormat()));
	mpPixelFormat->setEnabled(true);

	mpExposureTime_us->setText(QString::number(mpModel->exposureTime_us(), 'f', 2));
	mpExposureTime_us->setEnabled(true);

	mpExposureAutoMode->setCurrentIndex(static_cast<int>(mpModel->exposureAuto()));
	mpExposureAutoMode->setEnabled(true);

	mpGain_dB->setText(QString::number(mpModel->gain_dB(), 'f', 2));
	mpGain_dB->setEnabled(true);

	mpGainAutoMode->setCurrentIndex(static_cast<int>(mpModel->gainAuto()));
	mpGainAutoMode->setEnabled(true);

	mpBalanceWhiteAutoMode->setCurrentIndex(static_cast<int>(mpModel->balanceWhiteAuto()));
	mpBalanceWhiteAutoMode->setEnabled(true);

	mpGammaEnable->setChecked(mpModel->gammaEnable());
	mpGammaEnable->setEnabled(true);

	mpGamma->setText(QString::number(mpModel->gamma(), 'f', 2));
	mpGamma->setEnabled(true);
}
