

#include "TeledyneFlirPropertyPage_Local.hpp"
#include "TeledyneFlirCameraModel.hpp"
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
//	uint8_t id = mpMode->text().toInt();
//	if (mDefaultCameraId != id)
//	{
//		sendSetCameraId(id);
//	}

	uint8_t fps = mpFrameRate_fps->text().toInt();
	if (mDefaultFrameRate_fps != fps)
	{
	}
}

void cTeledyneFlirPropertyPage_Local::reject()
{
	doCancel();
}


