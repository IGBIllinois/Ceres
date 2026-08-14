
#pragma once

#include <nlohmann/json.hpp>


/***********************************************************************/
/**  Axis Communications Experiment State Helper to Configure Camera  **/
/***********************************************************************/


class cAxisCommunicationsExperimentState_Configure
{
public:
	cAxisCommunicationsExperimentState_Configure();

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
/**   Axis Communications Experiment State Helper to Take Photo       **/
/***********************************************************************/

class cAxisCommunicationsExperimentState_TakePhoto
{
public:
	cAxisCommunicationsExperimentState_TakePhoto();

	bool configure(const nlohmann::json& stateDoc);

protected:
	bool mUpdateView = false;

	bool mTriggerPhoto = true;
};


