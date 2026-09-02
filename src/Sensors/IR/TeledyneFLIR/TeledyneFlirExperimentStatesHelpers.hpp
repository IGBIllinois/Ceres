
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
	int mImageWidth = -1;
	int mImageHeight = -1;
	int mCameraID = -1;

	bool mWaitingForMode = false;
	bool mWaitingForCameraID = false;
	bool mWaitingForResolution = false;
	bool mWaitingForFrameRate = false;
	bool mWaitingForInterval = false;
};


/***********************************************************************/
/**      Teledyne FLIR Experiment State Helper to Take Image          **/
/***********************************************************************/

class cTeledyneFlirExperimentHelper_TakeImage
{
public:
	cTeledyneFlirExperimentHelper_TakeImage();

	bool configure(const nlohmann::json& stateDoc);

protected:
	bool mUpdateView = false;

	bool mTriggerImage = true;

	int mNumOfImages = 1;
};


