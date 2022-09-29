
#pragma once

#include "ExperimentState.hpp"

#include <QString>
#include <QMessageBox>

#include <nlohmann/json.hpp>
#include <chrono>


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
	void stop() override {};
	eRESULT finished() override
	{
		return eRESULT::DONE;
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
	void stop() override;
	eRESULT finished() override;

private:
	std::chrono::time_point<std::chrono::steady_clock>	mStart;
	double mElapsedTime_sec;
	double mWaitTime_sec;
};


class cPauseExperimentStateDlg;

class cExperimentState_Pause : public QObject, public cExperimentState
{
	Q_OBJECT

public:
	cExperimentState_Pause();
	~cExperimentState_Pause();

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

signals:
	void showDlg();

private:
	cPauseExperimentStateDlg* mpDlg;
};


