
#pragma once


#include "SpidercamModel.hpp"
#include "SpidercamExperimentStates.hpp"
#include "SpidercamCtrlProxies.hpp"

#include <chrono>
#include <memory>


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

public:
	bool linkError() const;

	bool isBusy() const;
	bool isConsoleConnected() const;
	bool isMoving() const;
	bool isSetPointEnabled() const;
	bool isInScriptMode() const;
	bool isInError() const;

	const spidercam::sPosition_1_t& getLastKnownPosition() const;

	bool requestStop();

	bool sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
		uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg);

protected:
	void update() override;

private:
	void reset_x_to_pos(long pos);
	void reset_y_to_pos(long pos);
	void reset_z_to_pos(long pos);
	void update_flags();

private:
	double mX_mm = 0.0;
	double mY_mm = 0.0;
	double mZ_mm = 0.0;

	long mTargetX_mm = -1;
	long mTargetY_mm = -1;
	long mTargetZ_mm = -1;

	double mVx_mmps = 0.0;
	double mVy_mmps = 0.0;
	double mVz_mmps = 0.0;

	uint64_t mFlags = 0;

	std::chrono::high_resolution_clock::time_point mLastUpdateTime;

	std::unique_ptr<cSpidercamCtrlProxy<cSpidercamModel_sim>> mpProxy;
};





