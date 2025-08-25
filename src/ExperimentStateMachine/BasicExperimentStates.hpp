
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
		return QString("Loading experiment info...");
	}

	bool configure(const nlohmann::json& stateDoc) override { return true;  };

	bool recording() override 
	{
		return false;
	};

	bool initialize() override { return true; };
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

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

private:
	std::chrono::time_point<std::chrono::steady_clock>	mStart;
	double mElapsedTime_sec;
	double mWaitTime_sec;
	bool mRecording;
};


class cPauseExperimentStateDlg;

class cExperimentState_Pause : public QObject, public cExperimentState
{
	Q_OBJECT

public:
	cExperimentState_Pause();
	~cExperimentState_Pause();

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

signals:
	void showDlg();

private:
	cPauseExperimentStateDlg* mpDlg;
};


