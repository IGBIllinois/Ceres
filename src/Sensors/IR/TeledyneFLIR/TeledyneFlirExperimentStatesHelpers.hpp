
#pragma once

#include <nlohmann/json.hpp>


/***********************************************************************/
/**     Teledyne FLIR Experiment State Helper to Configure Camera     **/
/***********************************************************************/


class cTeledyneFlirExperimentHelper_Configure
{
public:
	cTeledyneFlirExperimentHelper_Configure();

	bool configure(const nlohmann::json& stateDoc);

protected:
	int mMode = -1;
	int mLapseInterval_ms = -1;
	double mFrameRate_fps = -1;

	bool mWaitingForConfiguration = true;

	bool mWaitingForMode = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


/***********************************************************************/
/**      Teledyne FLIR Experiment State Helper to Take Photo          **/
/***********************************************************************/

class cTeledyneFlirExperimentHelper_TakePhoto
{
public:
	cTeledyneFlirExperimentHelper_TakePhoto();

	bool configure(const nlohmann::json& stateDoc);

protected:
	bool mUpdateView = false;

	int mNumOfPhotos = 1;
};


