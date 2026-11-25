
#pragma once

#include <spidercam_connect/spidercam_types.hpp>

#include <QToolBar>
#include <QString>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QToolButton;
class QMenu;
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
	void updateLimits(spidercam::sWorkingDimensions limits);
	void updatePosition(spidercam::sPosition_1_t pos);

public slots:
	void experimentRunning();
	void experimentStopped();

private slots:
	void loadButtonPressed();
	void stopButtonPressed();

	// Slots associated with "Compute" menu actions
private slots:
	void onComputeGroundHeight();
	void onComputeSpidercamHeight();
	void onComputeSensorRange();
	void onComputeReferenceHeight();

private:
	double mMinX_mm = 0;
	double mMaxX_mm = 0;
	double mMinY_mm = 0;
	double mMaxY_mm = 0;
	double mMinHeight_mm = 0;
	double mMaxHeight_mm = 0;

	int32_t mX_mm = 0;
	int32_t mY_mm = 0;
	int32_t mZ_mm = 0;

	QMenu* mpComputeMenu = nullptr;

	QAction* mpLoadAction;
	QAction* mpStopAction;
};

