
#pragma once

#include "ExperimentState.hpp"
#include "TeledyneFlirExperimentStatesHelpers.hpp"
#include "TeledyneFlirExperimentStatesHelpers.hpp"

#include <QObject>

#include <optional>

// Forward Declarations
class cTeledyneFlirCameraModel;


/*******************************************************************/
/**    Base Class for Teledyne FLIR Local Experiment States      **/
/*******************************************************************/

class cTeledyneFlirCameraExperimentState_Local : public cExperimentState
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


/***********************************************************************/
/**    Teledyne FLIR Experiment State to Save Current Camera State    **/
/***********************************************************************/

class cTeledyneFlirCamera_SaveState_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_SaveState_Local(QObject* parent = nullptr);

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
/**      Teledyne FLIR Experiment State to Restore Camera State       **/
/***********************************************************************/

class cTeledyneFlirCamera_RestoreState_Local : public cTeledyneFlirCameraExperimentState_Local
{
	Q_OBJECT

public:
	cTeledyneFlirCamera_RestoreState_Local(QObject* parent = nullptr);

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


/*******************************************************************/
/**    Teledyne FLIR Experiment States to Configure Camera        **/
/*******************************************************************/

class cTeledyneFlirCamera_Configure_Local : public cTeledyneFlirCameraExperimentState_Local, protected cTeledyneFlirExperimentHelper_Configure
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
};


/*******************************************************************/
/**       Teledyne FLIR Experiment States to Take Photo           **/
/*******************************************************************/

class cTeledyneFlirCamera_TakePhoto_Local : public cTeledyneFlirCameraExperimentState_Local, protected cTeledyneFlirExperimentHelper_TakePhoto
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
	void takePhoto(bool update_view, bool auto_save);

public slots:
	void onPhotoTaken();

private:
	bool mTriggerImage = true;

	cExperimentState::eRESULT mResult = cExperimentState::eRESULT::WAITING;
};


