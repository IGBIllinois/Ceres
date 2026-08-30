

#include "AxisCommunicationsPropertyPage_Remote.hpp"
#include "AxisCommunicationsIDs.hpp"
#include "AxisCommunicationsUtils.hpp"
#include "AxisCommunicationsExperimentStates_Remote.hpp"
#include "StringUtils.hpp"

#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QHBoxLayout>

namespace
{
	const QString ReTry = "ReConnect";
}


cAxisCommunicationsPropertyPage_Remote::cAxisCommunicationsPropertyPage_Remote(QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cAxisPropertiesNetEncoder(255)
{}

cExperimentState* cAxisCommunicationsPropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (nStringUtils::iequal(type, axis_communications_id))
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string cmd = entry["command"];

		if ((cmd == "save_state") || (cmd == "save state"))
		{
			auto* pState = new cAxisCommunications_SaveState_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if ((cmd == "restore_state") || (cmd == "restore state"))
		{
			auto* pState = new cAxisCommunications_RestoreState_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "configure")
		{
			auto* pState = new cAxisCommunications_Configure_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if ((cmd == "take_photo") || (cmd == "take photo"))
		{
			auto* pState = new cAxisCommunications_TakePhoto_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}

void cAxisCommunicationsPropertyPage_Remote::createWidgets()
{
	cAxisCommunicationsPropertyPage::createWidgets();

	mpButtons->addButton(ReTry, QDialogButtonBox::ButtonRole::HelpRole);
}

void cAxisCommunicationsPropertyPage_Remote::enableControls(bool enable)
{
	cAxisCommunicationsPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cAxisCommunicationsPropertyPage_Remote::buttonClicked(QAbstractButton* button)
{
	auto text = button->text();
	if (text == ReTry)
	{
		if (!mConnected)
		{
			if (!openConnection())
			{
				QMessageBox::warning(this, "Ceres",
					"Could not connect to the Axis Communications F44 controller.",
					QMessageBox::Ok);
			}
		}
		else
		{
			mReconnectActive = true;

			button->setEnabled(false);

			closeConnection();

			QTime delayTime = QTime::currentTime().addSecs(5);
			while (QTime::currentTime() < delayTime)
			{
				QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			}

			if (!openConnection())
			{
				QMessageBox::warning(this, "Ceres",
					"Could not connect to the Axis Communications F44 controller.",
					QMessageBox::Ok);
			}

			button->setEnabled(true);

			mReconnectActive = false;
		}

		return;
	}

	cAxisCommunicationsPropertyPage::buttonClicked(button);
}

void cAxisCommunicationsPropertyPage_Remote::onConnect()
{
	cAxisPropertiesNetEncoder::sendQueryStateMessage();
}

void cAxisCommunicationsPropertyPage_Remote::onGrabImagePressed()
{
	if (mpMode->currentIndex() == 0)
	{
		mpGrabImage->setEnabled(false);
		cAxisPropertiesNetEncoder::sendTakePhotoMessage(true);
	}
	else
		cAxisPropertiesNetEncoder::sendGrabImageMessage();
}

void cAxisCommunicationsPropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the Axis Communications F44 controller.",
			QMessageBox::Ok);
	}

	cAxisCommunicationsPropertyPage::showPage();
}

void cAxisCommunicationsPropertyPage_Remote::doOK()
{
	doApply();
	closeConnection();
	cAxisCommunicationsPropertyPage::doOK();
}

void cAxisCommunicationsPropertyPage_Remote::doCancel()
{
	closeConnection();
	cAxisCommunicationsPropertyPage::doCancel();
}

void cAxisCommunicationsPropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	auto image_size = mpImageSizes->currentText();
	auto is = axis::to_image_size(image_size.toStdString());
	if ((mDefaultImageWidth != is.width) || (mDefaultImageHeight != is.height))
	{
		sendSetImageSizeMessage(is.width, is.height);
	}

	uint8_t mode = mpMode->currentIndex();
	if (mDefaultMode != mode)
	{
		sendSetModeMessage(mode);
	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
		sendSetFrameRateMessage(fps);
	}

	uint32_t interval_ms = static_cast<uint32_t>(mpLapseInterval_s->text().toDouble() * 1000.0);
	if (mDefaultLapseInterval_ms != interval_ms)
	{
		sendSetLapseIntervalMessage(interval_ms);
	}
}

void cAxisCommunicationsPropertyPage_Remote::reject()
{
	doCancel();
}

/**
 * Message Handlers from the network decoder
 */
void cAxisCommunicationsPropertyPage_Remote::onModeMessage(uint8_t mode)
{
	if ((mode < 0) || (mode > 2))
		return;

	mDefaultMode = mode;
	mpMode->setCurrentIndex(mode);
}

void cAxisCommunicationsPropertyPage_Remote::onImageSizeMessage(uint16_t width, uint16_t height)
{
	mDefaultImageWidth = width;
	mDefaultImageHeight = height;

	QString image_size = QString::number(width);
	image_size += "x";
	image_size += QString::number(height);

	auto n = mpImageSizes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpImageSizes->itemText(i);
		if (0 == data.compare(image_size))
		{
			mpImageSizes->setCurrentIndex(i);
			break;
		}
	}
}

void cAxisCommunicationsPropertyPage_Remote::onFrameRateMessage(uint8_t fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mpFrameRate_fps->setText(QString::number(fps));
	mDefaultFrameRate_fps = fps;
}

void cAxisCommunicationsPropertyPage_Remote::onLapseIntervalMessage(uint32_t interval_ms)
{
	mDefaultLapseInterval_ms = interval_ms;
	mpLapseInterval_s->setText(QString::number(interval_ms * 0.001f));
}

void cAxisCommunicationsPropertyPage_Remote::onTakePhotoReplyMessage(bool error)
{
	mpGrabImage->setEnabled(true);
}

void cAxisCommunicationsPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cAxisPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cAxisCommunicationsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}


/*
 * Property Page for the Axis Communications F44 system
 */

cAxisCommunicationsPropertyPage_Remote_F44::cAxisCommunicationsPropertyPage_Remote_F44(QWidget* parent)
{}

void cAxisCommunicationsPropertyPage_Remote_F44::createWidgets()
{
	cAxisCommunicationsPropertyPage_Remote::createWidgets();

	mpCameraIdLabel = new QLabel("Camera ID:", this);
	mpCameraId = new QComboBox(this);
}

void cAxisCommunicationsPropertyPage_Remote_F44::doLayout(QVBoxLayout* pMainLayout)
{
	assert(pMainLayout);

	auto* idLayout = new QHBoxLayout();
	idLayout->addWidget(mpCameraIdLabel);
	idLayout->addWidget(mpCameraId);
	pMainLayout->addLayout(idLayout);

	cAxisCommunicationsPropertyPage_Remote::doLayout(pMainLayout);
}

void cAxisCommunicationsPropertyPage_Remote_F44::cameraIdTextChanged(const QString& text)
{
	auto id = text.toInt();

	if ((id < mMinCameraId) || (id > mMaxCameraId)) return;
}

void cAxisCommunicationsPropertyPage_Remote_F44::doApply()
{
	if (!mConnected)
		return;

	uint8_t id = mpCameraId->currentIndex() + mMinCameraId;
	if (mDefaultCameraId != id)
	{
		sendSetCameraIdMessage(id);
	}

	cAxisCommunicationsPropertyPage_Remote::doApply();
}

/**
 * Message Handlers from the network decoder
 */

void cAxisCommunicationsPropertyPage_Remote_F44::onCameraIdMessage(uint8_t id)
{
	if ((id < mMinCameraId) || (id > mMaxCameraId))
		return;

	mpCameraId->setCurrentIndex(id - mMinCameraId);
	mDefaultCameraId = id;
}

void cAxisCommunicationsPropertyPage_Remote_F44::onCurrentStateMessage(bool valid, uint8_t id,
	uint16_t width, uint16_t height, uint8_t fps)
{
	if (!valid) return;

	mMinCameraId = 1;
	mMaxCameraId = 4;

	mpCameraId->clear();

	for (auto i = mMinCameraId; i <= mMaxCameraId; i++)
	{
		mpCameraId->addItem(QString::number(i));
	}

	onCameraIdMessage(id);
	onImageSizeMessage(width, height);
	onFrameRateMessage(fps);
}

void cAxisCommunicationsPropertyPage_Remote_F44::onCurrentStateMessage(bool valid, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (!valid) return;

	mMinCameraId = min_id;
	mMaxCameraId = max_id;

	mpCameraId->clear();

	for (auto i = mMinCameraId; i <= mMaxCameraId; i++)
	{
		mpCameraId->addItem(QString::number(i));
	}

	onCameraIdMessage(active_id);
	onImageSizeMessage(width, height);
	onFrameRateMessage(fps);
}

void cAxisCommunicationsPropertyPage_Remote_F44::onCurrentStateMessage(bool valid, uint8_t mode, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint32_t interval_ms, uint8_t min_id, uint8_t max_id,
	std::optional<double> min_fps, std::optional<double> max_fps)
{
	if (!valid) return;

	mMinCameraId = min_id;
	mMaxCameraId = max_id;

	mpCameraId->clear();

	for (auto i = mMinCameraId; i <= mMaxCameraId; i++)
	{
		mpCameraId->addItem(QString::number(i));
	}

	onModeMessage(mode);
	onCameraIdMessage(active_id);
	onImageSizeMessage(width, height);
	onFrameRateMessage(fps);
	onLapseIntervalMessage(interval_ms);

	if (min_fps.has_value() && max_fps.has_value())
		mpFrameRate_fps->setValidator(new QDoubleValidator(min_fps.value(), max_fps.value(), 2));
}

