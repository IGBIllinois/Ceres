
#pragma once

#include <spidercam/spidercam_types.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <QString>

// Forward Declarations
class cSpidercamController;


class cSpidercamExperimentState
{
public:
	cSpidercamExperimentState() = default;
	virtual ~cSpidercamExperimentState() = default;

	virtual QString getStatusStr() = 0;

	virtual void configure(const nlohmann::json& stateDoc) = 0;

	virtual bool recording() = 0;

	virtual void initialize() = 0;
	virtual void run() = 0;
	virtual bool finished() = 0;
};


class cSpidercamExperimentState_Dummy : public cSpidercamExperimentState
{
public:
	cSpidercamExperimentState_Dummy() = default;

	QString getStatusStr() override
	{
		return QString();
	}

	void configure(const nlohmann::json& stateDoc) override {};

	bool recording() override 
	{
		return false;
	};

	void initialize() override {};
	void run() override {};
	bool finished() override
	{
		return true;
	}
};


class cSpidercamExperimentState_Delay : public cSpidercamExperimentState
{
public:
	cSpidercamExperimentState_Delay();

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	bool finished() override;

private:
	std::chrono::time_point<std::chrono::steady_clock>	mStart;
	double mElapsedTime_sec;
	double mWaitTime_sec;
};


class cSpidercamExperimentState_Movement : public cSpidercamExperimentState
{
public:
	cSpidercamExperimentState_Movement(const spidercam::sPosition& pos,
		cSpidercamController& controller, uint32_t tolerance_mm);

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	bool finished() override;

private:
	const spidercam::sPosition& mDollyPos;
	cSpidercamController& mController;

	bool mRecordData;

	uint32_t mX_mm;
	uint32_t mY_mm;
	uint32_t mZ_mm;

	uint32_t mTolerance_mm;

	double mSpeed_mmps;
	double mPan_deg;
	double mTilt_deg;
};

