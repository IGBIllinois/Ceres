
#include "SpidercamToolbar.hpp"

#include <QToolButton>

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


