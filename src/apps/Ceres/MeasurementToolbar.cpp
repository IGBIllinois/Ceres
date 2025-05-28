
#include "MeasurementToolbar.hpp"

#include <QToolButton>

cMeasurementToolbar::cMeasurementToolbar(QWidget* parent)
	: QToolBar(tr("Measurement"), parent),
		mpLoadAction(nullptr), mpStartAction(nullptr),
		mpPauseAction(nullptr), mpStopAction(nullptr)
{
	setFloatable(false);
	setMovable(true);

	mpLoadAction = new QAction();
	mpLoadAction->setIcon(QIcon(":/ripe.illinois.edu/load_exp.png"));
	mpLoadAction->setStatusTip(tr("Load measurement..."));
	connect(mpLoadAction, &QAction::triggered, this, &cMeasurementToolbar::loadButtonPressed);
	this->addAction(mpLoadAction);

	mpStartAction = new QAction;
	mpStartAction->setIcon(QIcon(":/ripe.illinois.edu/start_exp.png"));
	mpStartAction->setStatusTip(tr("Run measurement..."));
	connect(mpStartAction, &QAction::triggered, this, &cMeasurementToolbar::startButtonPressed);
	this->addAction(mpStartAction);

	mpPauseAction = new QAction;
	mpPauseAction->setIcon(QIcon(":/ripe.illinois.edu/pause_exp.png"));
	mpPauseAction->setStatusTip(tr("Pause the currently running measurement"));
	connect(mpPauseAction, &QAction::triggered, this, &cMeasurementToolbar::pauseButtonPressed);
	this->addAction(mpPauseAction);

	mpStopAction = new QAction;
	mpStopAction->setIcon(QIcon(":/ripe.illinois.edu/stop_exp.png"));
	mpStopAction->setStatusTip(tr("Stop the currently running measurement"));
	connect(mpStopAction, &QAction::triggered, this, &cMeasurementToolbar::stopButtonPressed);
	this->addAction(mpStopAction);

	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(false);
}

void cMeasurementToolbar::measurementRunning()
{
	mpLoadAction->setEnabled(false);
	mpStartAction->setEnabled(false);
	mpPauseAction->setEnabled(true);
	mpStopAction->setEnabled(true);
}

void cMeasurementToolbar::measurementPaused()
{
	mpLoadAction->setEnabled(false);
	mpStartAction->setEnabled(true);
	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(true);
}

void cMeasurementToolbar::measurementStopped()
{
	mpLoadAction->setEnabled(true);
	mpStartAction->setEnabled(true);
	mpPauseAction->setEnabled(false);
	mpStopAction->setEnabled(false);
}

void cMeasurementToolbar::loadButtonPressed()
{
	emit loadSelected();
}

void cMeasurementToolbar::startButtonPressed()
{
	emit runSelected();
}

void cMeasurementToolbar::pauseButtonPressed()
{
	emit pauseSelected();
}

void cMeasurementToolbar::stopButtonPressed()
{
	emit stopSelected();
}


