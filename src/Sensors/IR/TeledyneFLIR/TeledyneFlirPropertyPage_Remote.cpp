

#include "TeledyneFlirPropertyPage_Remote.hpp"
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

namespace
{
	const QString ReTry = "ReConnect";
}


cTeledyneFlirPropertyPage_Remote::cTeledyneFlirPropertyPage_Remote(QWidget* parent)
	: cTeledyneFlirPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cTeledyneFlirPropertiesNetEncoder(255)
{}

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

	cTeledyneFlirPropertyPage::buttonClicked(button);
}

void cTeledyneFlirPropertyPage_Remote::onConnect()
{
	cTeledyneFlirPropertiesNetEncoder::sendQueryState();
}

void cTeledyneFlirPropertyPage_Remote::requestImage()
{
	cTeledyneFlirPropertiesNetEncoder::sendGrabImage();
}

void cTeledyneFlirPropertyPage_Remote::onCameraId(uint8_t id)
{
	if ((id == 0) || (id > 4))
		return;

	mpCameraId->setText(QString::number(id));
	mDefaultCameraId = id;
}

void cTeledyneFlirPropertyPage_Remote::onImageSize(uint16_t width, uint16_t height)
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

void cTeledyneFlirPropertyPage_Remote::onFrameRate(uint8_t fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mpFrameRate_fps->setText(QString::number(fps));
	mDefaultFrameRate_fps = fps;
}

void cTeledyneFlirPropertyPage_Remote::onCurrentState(bool valid, uint8_t id,
	uint16_t width, uint16_t height, uint8_t fps)
{
	if (!valid) return;

	onCameraId(id);
	onImageSize(width, height);
	onFrameRate(fps);
}

void cTeledyneFlirPropertyPage_Remote::onCurrentState(bool valid, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (!valid) return;

	onCameraId(active_id);
	onImageSize(width, height);
	onFrameRate(fps);

	mpCameraId->setValidator(new QIntValidator(min_id, max_id));
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

	uint8_t id = mpCameraId->text().toInt();
	if (mDefaultCameraId != id)
	{
		sendSetCameraId(id);
	}

	auto image_size = mpImageSizes->currentText();
	if (image_size.compare(mDefaultImageSize) != 0)
	{
//BAF		auto is = teledyne::to_image_size(image_size.toStdString());
//		sendSetImageSize(is.width, is.height);
	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
		sendSetFrameRate_fps(fps);
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

