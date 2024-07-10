
#pragma once

#include <QString>

#include <nlohmann/json.hpp>
#include <memory>


class cExperimentVariableTable;


class cExperimentState
{
public:
	enum class eRESULT 
	{
		WAITING,	// The state is not complete and the state machine should not advance
		DONE,		// The state is complete and the state machine should advance
		ABORT		// The state is requesting an abort of the experiment
	};

	cExperimentState() = default;
	virtual ~cExperimentState() = default;

	virtual QString getStatusStr() = 0;

	void attachVariableTable(std::weak_ptr<cExperimentVariableTable> vars)
	{
		mVariables = vars;
	}

	virtual bool configure(const nlohmann::json& stateDoc) = 0;
	virtual void cleanup() {};

	virtual bool needsDataFile() { return recording(); }
	virtual bool recording() = 0;

	virtual bool initialize() = 0;
	virtual void run() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual eRESULT finished() = 0;

protected:
	std::weak_ptr<cExperimentVariableTable> mVariables;
};


