
#pragma once

#include <nlohmann/json.hpp>
#include <chrono>
#include <QString>

class cExperimentState
{
public:
	cExperimentState() = default;
	virtual ~cExperimentState() = default;

	virtual QString getStatusStr() = 0;

	virtual void configure(const nlohmann::json& stateDoc) = 0;

	virtual bool recording() = 0;

	virtual void initialize() = 0;
	virtual void run() = 0;
	virtual void pause() = 0;
	virtual bool finished() = 0;
};


class cExperimentState_Dummy : public cExperimentState
{
public:
	cExperimentState_Dummy() = default;

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
	void pause() override {};
	bool finished() override
	{
		return true;
	}
};


class cExperimentState_Delay : public cExperimentState
{
public:
	cExperimentState_Delay();

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	void pause() override;
	bool finished() override;

private:
	std::chrono::time_point<std::chrono::steady_clock>	mStart;
	double mElapsedTime_sec;
	double mWaitTime_sec;
};


