
#pragma once


#include "SpidercamModel.hpp"
#include "SpidercamCtrl.hpp"
#include "SpidercamCtrlProxies.hpp"

#include "SpidercamExperimentStates.hpp"

#include <memory>

class cSpidercamModel_net : public cSpidercamModel
{
    Q_OBJECT

public:
	cSpidercamModel_net(QObject* parent = nullptr);
	virtual ~cSpidercamModel_net();

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

protected slots:
	void onConnectionStateChange(bool connected);

protected:
	void update() override;

	void updateState();
	void updateObstacleDistance();

private:
	bool mConnected = false;

private:
    cSpidercamController mController;
	std::unique_ptr<cSpidercamCtrlProxy<cSpidercamController>> mpProxy;
};




