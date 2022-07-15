
#pragma once

#include <QToolBar>
#include <QString>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE


class cSpidercamToolbar : public QToolBar
{
	Q_OBJECT

public:
	cSpidercamToolbar(QWidget* parent = nullptr);

signals:
	void loadSelected();
	void stopSelected();

public slots:
	void experimentRunning();
	void experimentStopped();

private slots:
	void loadButtonPressed();
	void stopButtonPressed();

private:
	QAction* mpLoadAction;
	QAction* mpStopAction;
};

