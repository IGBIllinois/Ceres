
#pragma once

#include "ExperimentState.hpp"
#include "GpsTypes.hpp"

#include <QObject>

#include <optional>


// Forward Declarations
class cGpsModel;


/*******************************************************************/
/**           Base Class for GPS Experiment States             **/
/*******************************************************************/

class cGpsExperimentState_Local : public cExperimentState
{
	Q_OBJECT

public:
	cGpsExperimentState_Local(cGpsModel* pModel, QObject* parent = nullptr);
	virtual ~cGpsExperimentState_Local();

	void cleanup() override;

	bool initialize() override;

protected:
	const cGpsModel* mpModel;
};

/*******************************************************************/
/**   GPS Experiment States to Control Reference Acquisition      **/
/*******************************************************************/

class cGpsReferenceAcquisition_Local : public cGpsExperimentState_Local
{
	Q_OBJECT

public:
	cGpsReferenceAcquisition_Local(cGpsModel* pModel, QObject* parent = nullptr);
	virtual ~cGpsReferenceAcquisition_Local();

	bool configure(const nlohmann::json& stateDoc) override;

	void run() override;
	void pause() override;
	void stop() override;

	eRESULT finished() override;

	QString getStatusStr() override;

signals:
	void queryReferenceState();
	void queryReferenceParameters();
	void queryReferenceData();

	void updateReferenceParameters(int min_integration_time_sec, int max_integration_time_sec, int error_threshold_mm);

	void startReferenceComputation();
	void abortReferenceCompute();

public slots:
	void onReferenceComplete();
	void referenceStateUpdated(::gps::eReferenceState state);

	void referenceParametersUpdated(int min_integration_time_sec, int max_integration_time_sec, int ref_error_threshold_mm);

protected:
	std::optional<std::uint16_t> mDesiredMinIntegrationTime_sec;
	std::optional<std::uint16_t> mDesiredMaxIntegrationTime_sec;
	std::optional<std::uint16_t> mDesiredRefErrorThreshold_mm;

	std::uint16_t mCurrentMinIntegrationTime_sec = 0;
	std::uint16_t mCurrentMaxIntegrationTime_sec = 0;
	std::uint16_t mCurrentRefErrorThreshold_mm = 0;

	bool mHasReferenceParameters = false;

	enum class eSTATE { WAIT_FOR_CONNECT, WAIT_FOR_STATE, WAIT_FOR_STATE_UPDATE, WAIT_FOR_REFERENCE, COMPLETE, ERROR };
	eSTATE mState = eSTATE::WAIT_FOR_CONNECT;
};

