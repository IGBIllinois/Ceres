
#pragma once

#include "ExperimentState.hpp"
#include "MarkerInterfaces.hpp"

/**
 * Markers in the data file for the beginning and ending of a measurement scan
 */
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


/**
 * Markers in the data file for the beginning and ending of a reference scan
 */
class cExperimentState_MarkStartReference : public cExperimentState
{
public:
	cExperimentState_MarkStartReference(iMarkerStartReference* pMarkerStartReference);

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
	iMarkerStartReference* const mpMarkerStartReference;
};


class cExperimentState_MarkEndReference : public cExperimentState
{
public:
	cExperimentState_MarkEndReference(iMarkerEndReference* pMarkerEndReference);

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
	iMarkerEndReference* const mpMarkerEndReference;
};



