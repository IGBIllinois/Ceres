

#include "LucidVisionLabsRgbPropertyPage_Remote.hpp"
#include "LucidVisionLabsRgbUtils.hpp"

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


cAxisCommunicationsPropertyPage_Remote::cAxisCommunicationsPropertyPage_Remote(QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cAxisPropertiesNetEncoder(255)
{}

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
	cAxisPropertiesNetEncoder::sendQueryState();
}

void cAxisCommunicationsPropertyPage_Remote::requestImage()
{
	cAxisPropertiesNetEncoder::sendGrabImage();
}

void cAxisCommunicationsPropertyPage_Remote::onCameraId(uint8_t id)
{
	if ((id == 0) || (id > 4))
		return;

	mpCameraId->setText(QString::number(id));
	mDefaultCameraId = id;
}

void cAxisCommunicationsPropertyPage_Remote::onImageSize(uint16_t width, uint16_t height)
{
	QString image_size = QString::number(width);
	image_size += "x";
	image_size += QString::number(height);

	auto n = mpImageSizes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpImageSizes->itemText(i);
		if (0 == data.compare(image_size))
		{
			mDefaultImageSize = image_size;
			mpImageSizes->setCurrentIndex(i);
			break;
		}
	}
}

void cAxisCommunicationsPropertyPage_Remote::onFrameRate(uint8_t fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mpFrameRate_fps->setText(QString::number(fps));
	mDefaultFrameRate_fps = fps;
}

void cAxisCommunicationsPropertyPage_Remote::onCurrentState(bool valid, uint8_t id,
	uint16_t width, uint16_t height, uint8_t fps)
{
	if (!valid) return;

	onCameraId(id);
	onImageSize(width, height);
	onFrameRate(fps);
}

void cAxisCommunicationsPropertyPage_Remote::onCurrentState(bool valid, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (!valid) return;

	onCameraId(active_id);
	onImageSize(width, height);
	onFrameRate(fps);

	mpCameraId->setValidator(new QIntValidator(min_id, max_id));
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

	uint8_t id = mpCameraId->text().toInt();
	if (mDefaultCameraId != id)
	{
		sendSetCameraId(id);
	}

	auto image_size = mpImageSizes->currentText();
	if (image_size.compare(mDefaultImageSize) != 0)
	{
		auto is = axis::to_image_size(image_size.toStdString());
		sendSetImageSize(is.width, is.height);
	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
		sendSetFrameRate_fps(fps);
	}
}

void cAxisCommunicationsPropertyPage_Remote::reject()
{
	doCancel();
}

void cAxisCommunicationsPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cAxisPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cAxisCommunicationsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

