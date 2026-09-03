
#pragma once

#include "ExperimentState.hpp"
#include "AxisCommunicationsExperimentStatesHelpers.hpp"

#include <QObject>

#include <optional>

// Forward Declarations
class cAxisCommunicationsModel;


/*********************************************************************************/
/**    Base Class for Axis Communications Camera Local Experiment States      **/
/*********************************************************************************/

class cAxisCommunicationsExperimentState_Local : public QObject, public cExperimentState
{
	Q_OBJECT

public:
	cAxisCommunicationsExperimentState_Local(cAxisCommunicationsModel* pModel, QObject* parent = nullptr);
	virtual ~cAxisCommunicationsExperimentState_Local();

	void cleanup() override;

	bool initialize() override;

protected:
	const cAxisCommunicationsModel* mpModel;
};


/***********************************************************************/
/** Axis Communications Experiment State to Save Current Camera State **/
/***********************************************************************/

class cAxisCommunications_SaveState_Local : public cAxisCommunicationsExperimentState_Local
{
	Q_OBJECT

public:
	cAxisCommunications_SaveState_Local(QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true; };
	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void requestSaveState();

private:
	eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


/***********************************************************************/
/**   Axis Communications Experiment State to Restore Camera State    **/
/***********************************************************************/

class cAxisCommunications_RestoreState_Local : public cAxisCommunicationsExperimentState_Local
{
	Q_OBJECT

public:
	cAxisCommunications_RestoreState_Local(QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true; };
	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void requestRestoreState();

private:
	eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


/***********************************************************************/
/**   Axis Communications Experiment State to Configure Camera       **/
/***********************************************************************/

class cAxisCommunications_Configure_Local : public cAxisCommunicationsExperimentState_Local, protected cAxisCommunicationsExperimentHelper_Configure
{
	Q_OBJECT

public:
	cAxisCommunications_Configure_Local(cAxisCommunicationsModel* pModel, QObject* parent = nullptr);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override { return false; };

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

signals:
	void requestMode(int mode);
	void requestCameraID(int id);
	void requestImageSize(int width, int height);
	void requestFrameRate_Hz(double frame_rate_hz);
	void requestLapseInterval_ms(uint32_t frame_interval_ms);

public slots:
	void onModeChange(int mode);
	void onCameraIdChange(int id);
	void onLapseIntervalChange(int interval_ms);
	void onFrameRateChange(int rate_fps);
	void onImageSizeChange(int width, int height);

private:
	bool mUpdateConfiguration = true;
};


/***********************************************************************/
/**   Axis Communications Experiment State to Take Photo Camera       **/
/***********************************************************************/

class cAxisCommunications_TakePhoto_Local : public cAxisCommunicationsExperimentState_Local, protected cAxisCommunicationsExperimentHelper_TakePhoto
{
	Q_OBJECT

public:
	cAxisCommunications_TakePhoto_Local(cAxisCommunicationsModel* pModel, QObject* parent = nullptr);

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
	void takePhoto(bool update_view, bool auto_save);

public slots:
	void onPhotoTaken();

private:
	bool mTriggerPhoto = true;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


