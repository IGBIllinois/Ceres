
#pragma once

#include "ExperimentState.hpp"
#include "ExperimentStateRemoteInterface.hpp"

// Forward Declarations
class cHySpexSWIR_384_PropertyPage_Remote;
class cHySpexVNIR_3000N_PropertyPage_Remote;

/*******************************************************************/
/**           Base Class for Ouster Experiment States             **/
/*******************************************************************/

class cOuster_Properties_Remote : public cExperimentState, public cExperimentStateRemoteInterface
{
	Q_OBJECT

public:
	cOuster_Properties_Remote();
	~cOuster_Properties_Remote();

	bool configure(const nlohmann::json& stateDoc) override;
	void cleanup() override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

protected:
	std::string mHostname;
	std::string mLocalIpAddress;
	bool mUse_IpV6 = false;
	uint16_t   mPort = 0;
};



