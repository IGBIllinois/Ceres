
#pragma once

#include "ExperimentState.hpp"

#include <QObject>

#include <optional>

// Forward Declarations
class cLucidVisionLabsRgbModel;


/*********************************************************************************/
/**    Base Class for Lucid Vision Labs RGB Camera Local Experiment States      **/
/*********************************************************************************/

class cLucidVisionLabsRgbCameraExperimentState_Local : public cExperimentState
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCameraExperimentState_Local(cLucidVisionLabsRgbModel* pModel, QObject* parent = nullptr);
	virtual ~cLucidVisionLabsRgbCameraExperimentState_Local();

	void cleanup() override;

	bool initialize() override;

protected:
	const cLucidVisionLabsRgbModel* mpModel;
};


/***********************************************************************/
/**    Lucid Vision Labs Experiment States to Configure Camera        **/
/***********************************************************************/

class cLucidVisionLabsRgbCamera_Configure_Local : public cLucidVisionLabsRgbCameraExperimentState_Local
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCamera_Configure_Local(cLucidVisionLabsRgbModel* pModel, QObject* parent = nullptr);

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


/***********************************************************************/
/**       Lucid Vision Labs Experiment States to Take Photo           **/
/***********************************************************************/

class cLucidVisionLabsRgbCamera_TakePhoto_Local : public cLucidVisionLabsRgbCameraExperimentState_Local
{
	Q_OBJECT

public:
	cLucidVisionLabsRgbCamera_TakePhoto_Local(cLucidVisionLabsRgbModel* pModel, QObject* parent = nullptr);

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


