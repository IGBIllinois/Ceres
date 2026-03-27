
#pragma once

#include "ExperimentState.hpp"

#include <QObject>

#include <optional>

// Forward Declarations
class cTeledyneFlirCameraModel;


/*******************************************************************/
/**    Base Class for Teledyne FLIR Local Experiment States      **/
/*******************************************************************/

class cTeledyneFlirCameraExperimentState_Local : public QObject, public cExperimentState
{
	Q_OBJECT

public:
	cTeledyneFlirCameraExperimentState_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);
	virtual ~cTeledyneFlirCameraExperimentState_Local();

	void cleanup() override;

	bool initialize() override;

protected:
	const cTeledyneFlirCameraModel* mpModel;
};


class cTeledyneFlirCamera_Configure_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_Configure_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void requestMode(int mode);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestLapseInterval_ms(uint32_t frame_interval_ms);

public slots:
	void modeChanged(int mode);
	void lapseIntervalChanged(int interval_ms);
	void frameRateChanged(double rate_fps);

private:
	int mMode = 0;
	int mLapseInterval_ms = 0;
	double mFrameRate_fps = 0;

	bool mUpdateConfiguration = true;

	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


class cTeledyneFlirCamera_TakePhoto_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_TakePhoto_Local(cTeledyneFlirCameraModel* pModel, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return true; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void updateView();
	void takePhoto(bool update_view);

public slots:
	void onPhotoTaken();

private:
	bool mUpdateView = false;

	bool mTriggerPhoto = true;
	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


