
#include "ExperimentToolbar.hpp"

#include <QToolButton>

cExperimentToolbar::cExperimentToolbar(QWidget* parent)
	: QToolBar(tr("Experiment"), parent),
		mpLoadAction(nullptr), mpStartAction(nullptr),
		mpPauseAction(nullptr), mpStopAction(nullptr)
{
	setFloatable(false);
	setMovable(true);

	mpLoadAction = new QAction();
	mpLoadAction->setIcon(QIcon(":/ripe.illinois.edu/load_exp.png"));
	mpLoadAction->setStatusTip(tr("Load experiment..."));
	connect(mpLoadAction, &QAction::triggered, this, &cExperimentToolbar::loadButtonPressed);
	this->addAction(mpLoadAction);

	mpStartAction = new QAction;
	mpStartAction->setIcon(QIcon(":/ripe.illinois.edu/start_exp.png"));
	mpStartAction->setStatusTip(tr("Run experiment..."));
	connect(mpStartAction, &QAction::triggered, this, &cExperimentToolbar::startButtonPressed);
	this->addAction(mpStartAction);

	mpPauseAction = new QAction;
	mpPauseAction->setIcon(QIcon(":/ripe.illinois.edu/pause_exp.png"));
	mpPauseAction->setStatusTip(tr("Pause the currently running experiment"));
	connect(mpPauseAction, &QAction::triggered, this, &cExperimentToolbar::pauseButtonPressed);
	this->addAction(mpPauseAction);

	mpStopAction = new QAction;
	mpStopAction->setIcon(QIcon(":/ripe.illinois.edu/stop_exp.png"));
	mpStopAction->setStatusTip(tr("Stop the currently running experiment"));
	connect(mpStopAction, &QAction::triggered, this, &cExperimentToolbar::stopButtonPressed);
	this->addAction(mpStopAction);

	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(false);
}

void cExperimentToolbar::experimentRunning()
{
	mpLoadAction->setEnabled(false);
	mpStartAction->setEnabled(false);
	mpPauseAction->setEnabled(true);
	mpStopAction->setEnabled(true);
}

void cExperimentToolbar::experimentPaused()
{
	mpLoadAction->setEnabled(false);
	mpStartAction->setEnabled(true);
	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(true);
}

void cExperimentToolbar::experimentStopped()
{
	mpLoadAction->setEnabled(true);
	mpStartAction->setEnabled(true);
	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(false);
}

void cExperimentToolbar::loadButtonPressed()
{
	emit loadSelected();
}

void cExperimentToolbar::startButtonPressed()
{
	emit runSelected();
}

void cExperimentToolbar::pauseButtonPressed()
{
	emit pauseSelected();
}

void cExperimentToolbar::stopButtonPressed()
{
	emit stopSelected();
}


