
#pragma once


#include "SpidercamModel.hpp"


class cSpidercamModel_sim : public cSpidercamModel
{
    Q_OBJECT

public:
	cSpidercamModel_sim(QObject* parent = nullptr);
	virtual ~cSpidercamModel_sim();

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    void configure(const nlohmann::json& jsonCfg) override;

	/*
	 * Create a experiment state for the state machine
	 */
	cExperimentState* createState(const std::string& type, const nlohmann::json& expState) override;

	/*
	 * Is the system ready to run an experiment?
	*/
	bool systemReady() const override;


	bool startCommunications() override;
	void stopCommunications() override;

protected:
	void update() override;


protected:

	cIntervalTimer	mTimer;
};





