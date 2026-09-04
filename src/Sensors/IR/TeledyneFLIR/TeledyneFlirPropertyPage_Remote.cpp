

#include "TeledyneFlirPropertyPage_Remote.hpp"
#include "TeledyneFlirIDs.hpp"
#include "TeledyneFlirUtils.hpp"
#include "TeledyneFlirExperimentStates_Remote.hpp"
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

namespace
{
	const QString ReTry = "ReConnect";
}


cTeledyneFlirPropertyPage_Remote::cTeledyneFlirPropertyPage_Remote(QWidget* parent)
	: cTeledyneFlirPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cTeledyneFlirPropertiesNetEncoder(255)
{}


cExperimentState* cTeledyneFlirPropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (nStringUtils::iequal(type, teledyne_flir_id))
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string cmd = entry["command"];

		if ((cmd == "save_state") || (cmd == "save state"))
		{
			auto* pState = new cTeledyneFlirCamera_SaveState_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if ((cmd == "restore_state") || (cmd == "restore state"))
		{
			auto* pState = new cTeledyneFlirCamera_RestoreState_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "configure")
		{
			auto* pState = new cTeledyneFlirCamera_Configure_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "take photo")
		{
			auto* pState = new cTeledyneFlirCamera_TakePhoto_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}

void cTeledyneFlirPropertyPage_Remote::createWidgets()
{
	cTeledyneFlirPropertyPage::createWidgets();

	mpButtons->addButton(ReTry, QDialogButtonBox::ButtonRole::HelpRole);
}

void cTeledyneFlirPropertyPage_Remote::enableControls(bool enable)
{
	cTeledyneFlirPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cTeledyneFlirPropertyPage_Remote::buttonClicked(QAbstractButton* button)
{
	auto text = button->text();
	if (text == ReTry)
	{
		if (!mConnected)
		{
			if (!openConnection())
			{
				QMessageBox::warning(this, "Ceres",
					"Could not connect to the Teledyne FLIR controller.",
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
					"Could not connect to the Teledyne FLIR controller.",
					QMessageBox::Ok);
			}

			button->setEnabled(true);

			mReconnectActive = false;
		}

		return;
	}

	cTeledyneFlirPropertyPage::buttonClicked(button);
}

void cTeledyneFlirPropertyPage_Remote::onConnect()
{
	cTeledyneFlirPropertiesNetEncoder::sendQueryStateMessage();
}

void cTeledyneFlirPropertyPage_Remote::onGrabImagePressed()
{
	if (mpMode->currentIndex() == 0)
	{
		mpGrabImage->setEnabled(false);
		cTeledyneFlirPropertiesNetEncoder::sendTakePhotoMessage(true);
	}
	else
		cTeledyneFlirPropertiesNetEncoder::sendGrabImageMessage();
}

void cTeledyneFlirPropertyPage_Remote::onModeMessage(uint8_t mode)
{
	if ((mode < 0) || (mode > 2))
		return;

	mDefaultMode = mode;
	mpMode->setCurrentIndex(mode);
}

void cTeledyneFlirPropertyPage_Remote::onImageSizeMessage(uint16_t width, uint16_t height)
{
	QString image_size = QString::number(width);
	image_size += " x ";
	image_size += QString::number(height);

	mpImageSize->setText(image_size);
	mpImageSize->setEnabled(false);
}

void cTeledyneFlirPropertyPage_Remote::onFrameRateMessage(double fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mDefaultFrameRate_fps = fps;
	mpFrameRate_fps->setText(QString::number(fps));
}

void cTeledyneFlirPropertyPage_Remote::onLapseIntervalMessage(uint32_t interval_ms)
{
	mDefaultLapseInterval_ms = interval_ms;
	mpLapseInterval_s->setText(QString::number(interval_ms * 0.001f));
}

void cTeledyneFlirPropertyPage_Remote::onThermalRangeMessage(float min_value_K, float max_value_K)
{
	if ((min_value_K > 0) && (max_value_K > 0))
	{
		QString thermal_range = QString::number(min_value_K);
		thermal_range += " to ";
		thermal_range += QString::number(max_value_K);

		mpThermalRange->setText(thermal_range);
	}
	else
		mpThermalRange->setText("Unknown");

	mpThermalRange->setEnabled(false);
}

void cTeledyneFlirPropertyPage_Remote::onCurrentStateMessage(bool valid, uint8_t mode,
	uint16_t width, uint16_t height, double fps, uint32_t interval_ms,
	std::optional<double> min_fps, std::optional<double> max_fps,
	std::optional<float> min_K, std::optional<float> max_K)
{
	if (!valid) return;

	mDefaultMode = mode;
	mDefaultFrameRate_fps = fps;
	mDefaultLapseInterval_ms = interval_ms;

	onModeMessage(mode);
	onImageSizeMessage(width, height);
	onFrameRateMessage(fps);
	onLapseIntervalMessage(interval_ms);

	switch (mode)
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
	}

	if (min_fps.has_value() && max_fps.has_value())
		mpFrameRate_fps->setValidator(new QDoubleValidator(min_fps.value(), max_fps.value(), 2));

	if (min_K.has_value() && max_K.has_value())
	{
		QString thermal_range = QString::number(min_K.value());
		thermal_range += " to ";
		thermal_range += QString::number(max_K.value());

		mpThermalRange->setText(thermal_range);
	}
	else
		mpThermalRange->setText("Unknown");
}

void cTeledyneFlirPropertyPage_Remote::onTakePhotoReplyMessage(bool error)
{
	mpGrabImage->setEnabled(true);
}

void cTeledyneFlirPropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the Axis Communications F44 controller.",
			QMessageBox::Ok);
	}

	cTeledyneFlirPropertyPage::showPage();
}

void cTeledyneFlirPropertyPage_Remote::doOK()
{
	doApply();
	closeConnection();
	cTeledyneFlirPropertyPage::doOK();
}

void cTeledyneFlirPropertyPage_Remote::doCancel()
{
	closeConnection();
	cTeledyneFlirPropertyPage::doCancel();
}

void cTeledyneFlirPropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	uint8_t mode = static_cast<uint8_t>(mpMode->currentIndex());
	if ((mDefaultMode < 200) && (mDefaultMode != mode))
	{
		sendSetModeMessage(mode);
	}

	double fps = mpFrameRate_fps->text().toDouble();
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

void cTeledyneFlirPropertyPage_Remote::reject()
{
	doCancel();
}

void cTeledyneFlirPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cTeledyneFlirPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cTeledyneFlirPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

