

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


cLucidVisionLabsRgbPropertyPage_Remote::cLucidVisionLabsRgbPropertyPage_Remote(QWidget* parent)
	: cLucidVisionLabsRgbPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cLucidRgbPropertiesNetEncoder(255)
{}

void cLucidVisionLabsRgbPropertyPage_Remote::createWidgets()
{
	cLucidVisionLabsRgbPropertyPage::createWidgets();

	mpButtons->addButton(ReTry, QDialogButtonBox::ButtonRole::HelpRole);
}

void cLucidVisionLabsRgbPropertyPage_Remote::enableControls(bool enable)
{
	cLucidVisionLabsRgbPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cLucidVisionLabsRgbPropertyPage_Remote::buttonClicked(QAbstractButton* button)
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

	cLucidVisionLabsRgbPropertyPage::buttonClicked(button);
}

void cLucidVisionLabsRgbPropertyPage_Remote::onConnect()
{
	cLucidRgbPropertiesNetEncoder::sendQueryState();
}

void cLucidVisionLabsRgbPropertyPage_Remote::requestImage()
{
	cLucidRgbPropertiesNetEncoder::sendGrabImage();
}

void cLucidVisionLabsRgbPropertyPage_Remote::onCameraId(uint8_t id)
{
	if ((id == 0) || (id > 4))
		return;

	mpCameraId->setText(QString::number(id));
	mDefaultCameraId = id;
}

void cLucidVisionLabsRgbPropertyPage_Remote::onImageSize(uint16_t width, uint16_t height)
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

void cLucidVisionLabsRgbPropertyPage_Remote::onFrameRate(uint8_t fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mpFrameRate_fps->setText(QString::number(fps));
	mDefaultFrameRate_fps = fps;
}

void cLucidVisionLabsRgbPropertyPage_Remote::onCurrentState(bool valid, uint8_t id,
	uint16_t width, uint16_t height, uint8_t fps)
{
	if (!valid) return;

	onCameraId(id);
	onImageSize(width, height);
	onFrameRate(fps);
}

void cLucidVisionLabsRgbPropertyPage_Remote::onCurrentState(bool valid, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (!valid) return;

	onCameraId(active_id);
	onImageSize(width, height);
	onFrameRate(fps);

	mpCameraId->setValidator(new QIntValidator(min_id, max_id));
}

void cLucidVisionLabsRgbPropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the Axis Communications F44 controller.",
			QMessageBox::Ok);
	}

	cLucidVisionLabsRgbPropertyPage::showPage();
}

void cLucidVisionLabsRgbPropertyPage_Remote::doOK()
{
	doApply();
	closeConnection();
	cLucidVisionLabsRgbPropertyPage::doOK();
}

void cLucidVisionLabsRgbPropertyPage_Remote::doCancel()
{
	closeConnection();
	cLucidVisionLabsRgbPropertyPage::doCancel();
}

void cLucidVisionLabsRgbPropertyPage_Remote::doApply()
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
		auto is = lucid::to_image_size(image_size.toStdString());
		sendSetImageSize(is.width, is.height);
	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
		sendSetFrameRate_fps(fps);
	}
}

void cLucidVisionLabsRgbPropertyPage_Remote::reject()
{
	doCancel();
}

void cLucidVisionLabsRgbPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cLucidRgbPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cLucidVisionLabsRgbPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

