
#pragma once

#include <QToolBar>
#include <QString>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE


class cMeasurementToolbar : public QToolBar
{
	Q_OBJECT

public:
	cMeasurementToolbar(QWidget* parent = nullptr);

signals:
	void loadSelected();
	void runSelected();
	void pauseSelected();
	void stopSelected();

public slots:
	void measurementRunning();
	void measurementPaused();
	void measurementStopped();

private slots:
	void loadButtonPressed();
	void startButtonPressed();
	void pauseButtonPressed();
	void stopButtonPressed();

private:
	QAction* mpLoadAction;
	QAction* mpStartAction;
	QAction* mpPauseAction;
	QAction* mpStopAction;
};

