
#pragma once

#include "ExperimentState.hpp"
#include "MarkerInterfaces.hpp"


class cExperimentState_MarkStartPosition : public cExperimentState
{
public:
	cExperimentState_MarkStartPosition(iMarkerStartPosition* pMarkerStartPosition);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true;  };

	bool recording() override { return false; }
	bool initialize() override { return true; }

	void run() override;
	void pause() override {};
	void stop() override {};

	eRESULT finished() override
	{
		return eRESULT::DONE;
	}

private:
	iMarkerStartPosition* const mpMarkerStartPosition;
};


class cExperimentState_MarkEndPosition : public cExperimentState
{
public:
	cExperimentState_MarkEndPosition(iMarkerEndPosition* pMarkerEndPosition);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override { return true; };

	bool recording() override { return false; }
	bool initialize() override { return true; }

	void run() override;
	void pause() override {};
	void stop() override {};

	eRESULT finished() override
	{
		return eRESULT::DONE;
	}

private:
	iMarkerEndPosition* const mpMarkerEndPosition;
};


