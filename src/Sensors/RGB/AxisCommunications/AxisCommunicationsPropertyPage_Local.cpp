

#include "AxisCommunicationsPropertyPage_Local.hpp"
#include "AxisCommunicationsIDs.hpp"
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsUtils.hpp"
#include "AxisCommunicationsExperimentStates_Local.hpp"
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


//
// Control of the Axis Communication cameras
//

cAxisCommunicationsPropertyPage_Local::cAxisCommunicationsPropertyPage_Local(cAxisCommunicationsModel* pModel, QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), mpModel(pModel)
{}

cExperimentState* cAxisCommunicationsPropertyPage_Local::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
/*
	if (nStringUtils::iequal(type, axis_communications_id))
	{
		std::string cmd = entry["command"];

		if (cmd == "configure")
		{
			auto* pState = new cAxisCommunications_Configure_Local(mpModel, parent);

			QObject::connect(pState, &cAxisCommunications_Configure_Local::requestMode,				mpModel, &cAxisCommunicationsModel::requestMode);
			QObject::connect(pState, &cAxisCommunications_Configure_Local::requestFrameRate_Hz,		mpModel, &cAxisCommunicationsModel::requestFrameRate_Hz);
			QObject::connect(pState, &cAxisCommunications_Configure_Local::requestLapseInterval_ms,	mpModel, &cAxisCommunicationsModel::requestLapseInterval_ms);

			QObject::connect(mpModel, &cAxisCommunicationsModel::modeChanged,			pState, &cAxisCommunications_Configure_Local::modeChanged);
			QObject::connect(mpModel, &cAxisCommunicationsModel::lapseIntervalChanged,	pState, &cAxisCommunications_Configure_Local::lapseIntervalChanged);
			QObject::connect(mpModel, &cAxisCommunicationsModel::frameRateChanged,		pState, &cAxisCommunications_Configure_Local::frameRateChanged);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "take photo")
		{
			auto* pState = new cAxisCommunications_TakePhoto_Local(mpModel, parent);

			QObject::connect(pState, qOverload<bool>(&cAxisCommunications_TakePhoto_Local::takePhoto),			mpModel, qOverload<bool>(&cAxisCommunicationsModel::takePhoto));
			QObject::connect(pState, qOverload<bool, bool>(&cAxisCommunications_TakePhoto_Local::takePhoto),	mpModel, qOverload<bool, bool>(&cAxisCommunicationsModel::takePhoto));
			QObject::connect(pState, &cAxisCommunications_TakePhoto_Local::updateView,							mpModel, &cAxisCommunicationsModel::requestImage);

			QObject::connect(mpModel, &cAxisCommunicationsModel::photoTaken, pState, &cAxisCommunications_TakePhoto_Local::onPhotoTaken);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}
*/

	return nullptr;
}

void cAxisCommunicationsPropertyPage_Local::createWidgets()
{
	cAxisCommunicationsPropertyPage::createWidgets();

	mpLapseInterval_s->setValidator(new QDoubleValidator(0.1, 3600.0, 1));
}

void cAxisCommunicationsPropertyPage_Local::doLayout()
{
	QString title = QString::fromStdString(mpModel->name());

	auto instance = mpModel->instance();

	if (!instance.empty())
	{
		title += ":";
		title += QString::fromStdString(instance);
	}

	setTitle(title);

	cAxisCommunicationsPropertyPage::doLayout();
}

void cAxisCommunicationsPropertyPage_Local::enableControls(bool enable)
{
	cAxisCommunicationsPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}


void cAxisCommunicationsPropertyPage_Local::onPhotoTaken()
{
}

void cAxisCommunicationsPropertyPage_Local::onModeChange(int mode)
{
	mDefaultMode = mode;

	if (!mpMode) return;

	switch (mode)
	{
	case cAxisCommunicationsModel::SINGLE:
		mpMode->setCurrentIndex(0);
		break;
	case cAxisCommunicationsModel::TIME_LAPSE:
		mpMode->setCurrentIndex(1);
		break;
	case cAxisCommunicationsModel::CONTINUOUS:
		mpMode->setCurrentIndex(2);
		break;
	}
}

void cAxisCommunicationsPropertyPage_Local::onLapseIntervalChange(int interval_ms)
{
	mDefaultLapseInterval_ms = interval_ms;

	if (!mpLapseInterval_s) return;

	double interval_s = interval_ms * 0.001;
	mpLapseInterval_s->setText(QString::number(interval_s, static_cast<char>(103), 4));
}

void cAxisCommunicationsPropertyPage_Local::onFrameRateChange(int rate_fps)
{
	mDefaultFrameRate_fps = rate_fps;

	if (!mpFrameRate_fps) return;

	mpFrameRate_fps->setText(QString::number(rate_fps));
}

void cAxisCommunicationsPropertyPage_Local::onImageSizeChange(int width, int height)
{
	mDefaultImageWidth = width;
	mDefaultImageHeight = height;

	if (!mpImageSizes) return;

	if ((width == 1920) && (height == 1080))
		mpImageSizes->setCurrentIndex(0);
	else if ((width == 1280) && (height == 720))
		mpImageSizes->setCurrentIndex(1);
	else if ((width == 1024) && (height == 768))
		mpImageSizes->setCurrentIndex(2);
	else if ((width == 1024) && (height == 640))
		mpImageSizes->setCurrentIndex(3);
	else if ((width == 800) && (height == 600))
		mpImageSizes->setCurrentIndex(4);
	else if ((width == 640) && (height == 480))
		mpImageSizes->setCurrentIndex(5);
}

void cAxisCommunicationsPropertyPage_Local::onGrabImagePressed()
{
	emit requestImage();
}

void cAxisCommunicationsPropertyPage_Local::showPage()
{
	mpModel->updateViews();

	cAxisCommunicationsPropertyPage::showPage();
}

void cAxisCommunicationsPropertyPage_Local::doOK()
{
	doApply();
	cAxisCommunicationsPropertyPage::doOK();
}

void cAxisCommunicationsPropertyPage_Local::doCancel()
{
	cAxisCommunicationsPropertyPage::doCancel();
}

void cAxisCommunicationsPropertyPage_Local::doApply()
{
	auto image_size = mpImageSizes->currentText();
	auto is = axis::to_image_size(image_size.toStdString());

	if ((mDefaultImageWidth != is.width) || (mDefaultImageHeight != is.height))
	{
		emit requestImageSize(is.width, is.height);
	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
		emit requestFrameRate_Hz(fps);
	}

	uint32_t interval_ms = static_cast<uint32_t>(mpLapseInterval_s->text().toDouble() * 1000.0);
	if (mDefaultLapseInterval_ms != interval_ms)
	{
		emit requestLapseInterval_ms(interval_ms);
	}
}

void cAxisCommunicationsPropertyPage_Local::reject()
{
	doCancel();
}


//
// Control of the Axis Communication F44 control unit
//

cAxisCommunicationsPropertyPage_Local_F44::cAxisCommunicationsPropertyPage_Local_F44(cAxisCommunicationsModel_F44* pModel, QWidget* parent)
	: cAxisCommunicationsPropertyPage_Local(pModel, parent), mpModel(pModel)
{}

void cAxisCommunicationsPropertyPage_Local_F44::onCameraIdChange(int id)
{
	mDefaultCameraId = id;

	if (!mpCameraId) return;

	mpCameraId->setText(QString::number(id));
}

void cAxisCommunicationsPropertyPage_Local_F44::doApply()
{
	uint8_t id = mpCameraId->text().toInt();
	if (mDefaultCameraId != id)
	{
		emit requestCameraID(id);
	}

	cAxisCommunicationsPropertyPage_Local::doApply();
}
