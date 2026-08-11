

#include "AxisCommunicationsPropertyPage_Local.hpp"
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsUtils.hpp"

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


cAxisCommunicationsPropertyPage_Local::cAxisCommunicationsPropertyPage_Local(cAxisCommunicationsModel* pModel, QWidget* parent)
	: cAxisCommunicationsPropertyPage(parent), mpModel(pModel)
{}

void cAxisCommunicationsPropertyPage_Local::createWidgets()
{
	cAxisCommunicationsPropertyPage::createWidgets();
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

void cAxisCommunicationsPropertyPage_Local::requestImage()
{
//	cAxisPropertiesNetEncoder::sendGrabImage();
}

/*
void cAxisCommunicationsPropertyPage_Local::onCameraId(uint8_t id)
{
	if ((id == 0) || (id > 4))
		return;

	mpCameraId->setText(QString::number(id));
	mDefaultCameraId = id;
}

void cAxisCommunicationsPropertyPage_Local::onImageSize(uint16_t width, uint16_t height)
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

void cAxisCommunicationsPropertyPage_Local::onFrameRate(uint8_t fps)
{
	if ((fps == 0) || (fps > 30))
		return;

	mpFrameRate_fps->setText(QString::number(fps));
	mDefaultFrameRate_fps = fps;
}

void cAxisCommunicationsPropertyPage_Local::onCurrentState(bool valid, uint8_t id,
	uint16_t width, uint16_t height, uint8_t fps)
{
	if (!valid) return;

	onCameraId(id);
	onImageSize(width, height);
	onFrameRate(fps);
}

void cAxisCommunicationsPropertyPage_Local::onCurrentState(bool valid, uint8_t active_id,
	uint16_t width, uint16_t height, uint8_t fps, uint8_t min_id, uint8_t max_id)
{
	if (!valid) return;

	onCameraId(active_id);
	onImageSize(width, height);
	onFrameRate(fps);

	mpCameraId->setValidator(new QIntValidator(min_id, max_id));
}
*/

void cAxisCommunicationsPropertyPage_Local::showPage()
{
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
/*
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
*/
}

void cAxisCommunicationsPropertyPage_Local::reject()
{
	doCancel();
}

