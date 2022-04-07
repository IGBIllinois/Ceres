
#pragma once

#include <QToolBar>
#include <QString>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE


class cExperimentToolbar : public QToolBar
{
public:
	cExperimentToolbar(QWidget* parent = nullptr);

signals:
	void loadSelected();
	void runSelected();
	void pauseSelected();
	void stopSelected();

private:
	QToolButton* mpLoadButton;
	QToolButton* mpStartButton;
	QToolButton* mpPauseButton;
	QToolButton* mpStopButton;
};

