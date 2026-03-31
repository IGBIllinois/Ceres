

#include "TeledyneFlirPropertyPage_Local.hpp"
#include "TeledyneFlirIDs.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"
#include "TeledyneFlirUtils.hpp"
#include "TeledyneFlirExperimentStates_Local.hpp"
#include "StringUtils.hpp"

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

cExperimentState* cTeledyneFlirPropertyPage_Local::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (nStringUtils::iequal(type, teledyne_flir_id))
	{
		std::string cmd = entry["command"];

		if (cmd == "configure")
		{
			auto* pState = new cTeledyneFlirCamera_Configure_Local(mpModel, parent);

			QObject::connect(pState, &cTeledyneFlirCamera_Configure_Local::requestMode,				mpModel, &cTeledyneFlirCameraModel::requestMode);
			QObject::connect(pState, &cTeledyneFlirCamera_Configure_Local::requestFrameRate_Hz,		mpModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
			QObject::connect(pState, &cTeledyneFlirCamera_Configure_Local::requestLapseInterval_ms, mpModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);

			QObject::connect(mpModel, &cTeledyneFlirCameraModel::modeChanged,			pState, &cTeledyneFlirCamera_Configure_Local::modeChanged);
			QObject::connect(mpModel, &cTeledyneFlirCameraModel::lapseIntervalChanged,	pState, &cTeledyneFlirCamera_Configure_Local::lapseIntervalChanged);
			QObject::connect(mpModel, &cTeledyneFlirCameraModel::frameRateChanged,		pState, &cTeledyneFlirCamera_Configure_Local::frameRateChanged);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "take photo")
		{
			auto* pState = new cTeledyneFlirCamera_TakePhoto_Local(mpModel, parent);

			QObject::connect(pState, &cTeledyneFlirCamera_TakePhoto_Local::takePhoto, mpModel, &cTeledyneFlirCameraModel::takePhoto);
			QObject::connect(pState, &cTeledyneFlirCamera_TakePhoto_Local::updateView, mpModel, &cTeledyneFlirCameraModel::requestImage);
			QObject::connect(mpModel, &cTeledyneFlirCameraModel::photoTaken, pState, &cTeledyneFlirCamera_TakePhoto_Local::onPhotoTaken);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}

void cTeledyneFlirPropertyPage_Local::createWidgets()
{
	cTeledyneFlirPropertyPage::createWidgets();

	mpLapseInterval_s->setValidator(new QDoubleValidator(0.1, 3600.0, 1));
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

void cTeledyneFlirPropertyPage_Local::onGrabImagePressed()
{
	emit requestImage();
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

	double interval_s = mpModel->lapseInterval_ms() * 0.001;
	mpLapseInterval_s->setText(QString::number(interval_s, static_cast<char>(103), 4));

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
		emit requestMode(cTeledyneFlirCameraModel::SINGLE);
		break;
	case 1:
	{
		uint32_t interval_ms = static_cast<uint32_t>(mpLapseInterval_s->text().toDouble() * 0.001);
		emit requestLapseInterval_ms(interval_ms);
		emit requestMode(cTeledyneFlirCameraModel::TIME_LAPSE);
		break;
	}
	case 2:
	{
		double fps = mpFrameRate_fps->text().toDouble();
		emit requestFrameRate_Hz(fps);
		emit requestMode(cTeledyneFlirCameraModel::CONTINUOUS);
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


