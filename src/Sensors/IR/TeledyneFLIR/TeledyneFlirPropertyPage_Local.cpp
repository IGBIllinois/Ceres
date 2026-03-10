

#include "TeledyneFlirPropertyPage_Local.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"
#include "TeledyneFlirUtils.hpp"

#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>


cTeledyneFlirPropertyPage_Local::cTeledyneFlirPropertyPage_Local(cTeledyneFlirCameraModel* pModel, QWidget* parent)
	: cTeledyneFlirPropertyPage(parent), mpModel(pModel)
{
	assert(mpModel);
}

void cTeledyneFlirPropertyPage_Local::createWidgets()
{
	cTeledyneFlirPropertyPage::createWidgets();

	mpFrameInterval_s->setValidator(new QDoubleValidator(0.1, 3600.0, 1));
}

void cTeledyneFlirPropertyPage_Local::doLayout()
{
	setTitle(QString::fromStdString(mpModel->name()));
	cTeledyneFlirPropertyPage::doLayout();
}

void cTeledyneFlirPropertyPage_Local::enableControls(bool enable)
{
	cTeledyneFlirPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cTeledyneFlirPropertyPage_Local::requestImage()
{
	mpModel->requestImage();
}

void cTeledyneFlirPropertyPage_Local::showPage()
{
	switch (mpModel->mode())
	{
	case cTeledyneFlirCameraModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cTeledyneFlirCameraModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cTeledyneFlirCameraModel::CONTINUOUS:
		mpMode->setCurrentIndex(2);
		break;
	}

	mpFrameRate_fps->setText(QString::number(mpModel->frameRate_Hz(), static_cast<char>(103), 4));

	double interval_s = mpModel->frameInterval_ms() * 0.001;
	mpFrameInterval_s->setText(QString::number(interval_s, static_cast<char>(103), 4));

	std::optional<float> minValue_K = mpModel->minThermalValue_K();
	std::optional<float> maxValue_K = mpModel->maxThermalValue_K();

	if (minValue_K.has_value() && maxValue_K.has_value())
	{
		QString thermal_range = QString::number(minValue_K.value());
		thermal_range += " to ";
		thermal_range += QString::number(maxValue_K.value());

		mpThermalRange->setText(thermal_range);
	}
	else
	{
		mpThermalRange->setText("Unknown");
	}

	cTeledyneFlirPropertyPage::showPage();
}

void cTeledyneFlirPropertyPage_Local::doOK()
{
	doApply();
	cTeledyneFlirPropertyPage::doOK();
}

void cTeledyneFlirPropertyPage_Local::doCancel()
{
	cTeledyneFlirPropertyPage::doCancel();
}

void cTeledyneFlirPropertyPage_Local::doApply()
{
	switch (mpMode->currentIndex())
	{
	case 0:
		mpModel->setMode(cTeledyneFlirCameraModel::SINGLE);
		break;
	case 1:
	{
		uint32_t interval_ms = static_cast<uint32_t>(mpFrameInterval_s->text().toDouble() * 0.001);
		mpModel->setFrameInterval_ms(interval_ms);
		mpModel->setMode(cTeledyneFlirCameraModel::TIME_LAPSE);
		break;
	}
	case 2:
	{
		double fps = mpFrameRate_fps->text().toDouble();
		mpModel->setFrameRate_Hz(fps);
		mpModel->setMode(cTeledyneFlirCameraModel::CONTINUOUS);
		break;
	}
	}

	cTeledyneFlirPropertyPage::doApply();
}

void cTeledyneFlirPropertyPage_Local::reject()
{
	doCancel();
}


cTeledyneFlirPropertyPage_T1K_Local::cTeledyneFlirPropertyPage_T1K_Local(cTeledyneFlirCameraModel_T1K* pModel, QWidget* parent)
	: cTeledyneFlirPropertyPage_Local(pModel, parent), mpModel(pModel)
{}

void cTeledyneFlirPropertyPage_T1K_Local::showPage()
{
	QString image_size = QString::number(mpModel->maxImageWidth());
	image_size += " x ";
	image_size += QString::number(mpModel->maxImageHeight());

	mpImageSize->setText(image_size);

	mpThermalRange->setEnabled(false);
	mpImageSize->setEnabled(false);

	cTeledyneFlirPropertyPage_Local::showPage();
}


