
#include "SpidercamToolbar.hpp"

#include "SpidercamGroundHeightDlg.hpp"
#include "SpidercamHeightDlg.hpp"

#include "RappFieldModel.hpp"

#include <QToolButton>
#include <QMenu>


cSpidercamToolbar::cSpidercamToolbar(QWidget* parent)
	: QToolBar(tr("Spidercam"), parent),
		mpLoadAction(nullptr), mpStopAction(nullptr)
{
	setFloatable(false);
	setMovable(true);

	mpLoadAction = new QAction();
	mpLoadAction->setText("Load/Unload");
	mpLoadAction->setStatusTip(tr("Move to the load/unload dolly position..."));
	connect(mpLoadAction, &QAction::triggered, this, &cSpidercamToolbar::loadButtonPressed);
	this->addAction(mpLoadAction);

	mpStopAction = new QAction;
	mpStopAction->setText("Stop");
	mpStopAction->setStatusTip(tr("Stop the currently running experiment"));
	connect(mpStopAction, &QAction::triggered, this, &cSpidercamToolbar::stopButtonPressed);
	this->addAction(mpStopAction);

    // Create a tool button
    QToolButton* menuButton = new QToolButton();
    menuButton->setText("Compute");
    menuButton->setPopupMode(QToolButton::MenuButtonPopup);

    // Create menu
    mpComputeMenu = new QMenu(menuButton);
    QAction* pMenuItem = nullptr;

    pMenuItem = new QAction(tr("Compute Ground Height"), this);
    pMenuItem->setStatusTip(tr("Compute the ground height at a given SpiderCam point"));
    connect(pMenuItem, &QAction::triggered, this, &cSpidercamToolbar::onComputeGroundHeight);
    mpComputeMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Compute SpiderCam Height"), this);
    pMenuItem->setStatusTip(tr("Compute the Spidercam heigth from single SpiderCam point and desired height above canopy"));
    connect(pMenuItem, &QAction::triggered, this, &cSpidercamToolbar::onComputeSpidercamHeight);
    mpComputeMenu->addAction(pMenuItem);

/*
    pMenuItem = new QAction(tr("Compute Sensor Height"), this);
    pMenuItem->setStatusTip(tr("Compute the Sensor heigth based on SpiderCam position"));
    connect(pMenuItem, &QAction::triggered, this, &cSpidercamToolbar::onComputeSensorRange);
    mpComputeMenu->addAction(pMenuItem);

    pMenuItem = new QAction(tr("Compute Reference Height"), this);
    pMenuItem->setStatusTip(tr("Compute the reference heigth based on SpiderCam current position"));
    connect(pMenuItem, &QAction::triggered, this, &cSpidercamToolbar::onComputeReferenceHeight);
    mpComputeMenu->addAction(pMenuItem);
*/

    // Attach menu to tool button
    menuButton->setMenu(mpComputeMenu);

    // Add tool button to toolbar
    this->addWidget(menuButton);
}

void cSpidercamToolbar::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;
}

void cSpidercamToolbar::updatePosition(spidercam::sPosition_1_t pos)
{
	mX_mm = pos.X_mm;
	mY_mm = pos.Y_mm;
	mZ_mm = pos.Z_mm;
}

void cSpidercamToolbar::experimentRunning()
{
	mpLoadAction->setEnabled(false);
}

void cSpidercamToolbar::experimentStopped()
{
	mpLoadAction->setEnabled(true);
}

void cSpidercamToolbar::loadButtonPressed()
{
	emit loadSelected();
}

void cSpidercamToolbar::stopButtonPressed()
{
	emit stopSelected();
}

void cSpidercamToolbar::onComputeGroundHeight()
{
    cSpidercamGroundHeightDlg dlg(nRFM::get_ground_model(), this);

    dlg.setPosition(mX_mm, mY_mm);

    dlg.exec();
}

void cSpidercamToolbar::onComputeSpidercamHeight()
{
    cSpidercamHeightDlg dlg(nRFM::get_ground_model(), this);

    dlg.setPosition(mX_mm, mY_mm);

    dlg.exec();
}

void cSpidercamToolbar::onComputeSensorRange()
{
//BAF    cComputeSensorRangeDlg dlg(nRFM::get_ground_model(), this);

//    dlg.exec();
}

void cSpidercamToolbar::onComputeReferenceHeight()
{
//BAF    cComputeReferenceHeightDlg dlg(nRFM::get_ground_model(), nRFM::get_aerial_model(), this);

//    dlg.setReferenceHeight_mm(nRFM::reference_height_mm());

//    auto result = dlg.exec();

//    if (result == QDialog::Accepted)
//    {
//        nRFM::set_reference_height_mm(dlg.getReferenceHeight_mm());
//    }
}

