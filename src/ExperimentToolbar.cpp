
#include "ExperimentToolbar.hpp"

#include <QToolButton>

cExperimentToolbar::cExperimentToolbar(QWidget* parent)
	: QToolBar(tr("Experiment"), parent),
		mpLoadButton(nullptr), mpStartButton(nullptr),
		mpPauseButton(nullptr), mpStopButton(nullptr)
{
	setFloatable(false);
	setMovable(true);

	mpLoadButton = new QToolButton;
	mpLoadButton->setIcon(QIcon(":/ripe.illinois.edu/load_exp.png"));
	this->addWidget(mpLoadButton);

	mpStartButton = new QToolButton;
	mpStartButton->setIcon(QIcon(":/ripe.illinois.edu/start_exp.png"));
	this->addWidget(mpStartButton);

	mpPauseButton = new QToolButton;
	mpPauseButton->setIcon(QIcon(":/ripe.illinois.edu/pause_exp.png"));
	this->addWidget(mpPauseButton);

	mpStopButton = new QToolButton;
	mpStopButton->setIcon(QIcon(":/ripe.illinois.edu/stop_exp.png"));
	this->addWidget(mpStopButton);
}

