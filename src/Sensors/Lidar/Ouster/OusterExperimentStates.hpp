
#pragma once

#include "ExperimentState.hpp"


// Forward Declarations
class cOusterPropertyPage;


class cOusterExperimentState_State : public cExperimentState
{
public:
	cOusterExperimentState_State(cOusterPropertyPage& propertyPage);

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

private:
};

