

#include "LucidVisionLabsRgbPropertyPage_Local.hpp"
#include "LucidVisionLabsRgbUtils.hpp"
#include "LucidVisionLabsRgbModel.hpp"

#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>


cLucidVisionLabsRgbPropertyPage_Local::cLucidVisionLabsRgbPropertyPage_Local(cLucidVisionLabsRgbModel* pModel, QWidget* parent)
	: cLucidVisionLabsRgbPropertyPage(parent), mpModel(pModel)
{}

void cLucidVisionLabsRgbPropertyPage_Local::createWidgets()
{
	cLucidVisionLabsRgbPropertyPage::createWidgets();
}

void cLucidVisionLabsRgbPropertyPage_Local::onGrabImagePressed()
{

}

void cLucidVisionLabsRgbPropertyPage_Local::exposureTimeSelectorChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::exposureAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::pixelFormatChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::gainAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::balanceWhiteAutoModeChanged(const QString&)
{

}

void cLucidVisionLabsRgbPropertyPage_Local::enableControls(bool enable)
{
	cLucidVisionLabsRgbPropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cLucidVisionLabsRgbPropertyPage_Local::buttonClicked(QAbstractButton* button)
{
	cLucidVisionLabsRgbPropertyPage::buttonClicked(button);
}

void cLucidVisionLabsRgbPropertyPage_Local::showPage()
{
	cLucidVisionLabsRgbPropertyPage::showPage();
}

void cLucidVisionLabsRgbPropertyPage_Local::doOK()
{
	doApply();
	cLucidVisionLabsRgbPropertyPage::doOK();
}

void cLucidVisionLabsRgbPropertyPage_Local::doCancel()
{
	cLucidVisionLabsRgbPropertyPage::doCancel();
}

void cLucidVisionLabsRgbPropertyPage_Local::doApply()
{
	auto currentMode = mpModel->mode();

	if (currentMode != mpMode->currentIndex())
	{
		emit changeMode(mpMode->currentIndex());
	}

//	double frameRate_Hz() const;
//	uint32_t lapseInterval_ms() const;

}

void cLucidVisionLabsRgbPropertyPage_Local::reject()
{
	doCancel();
}
