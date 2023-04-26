
#pragma once

#include "../ExperimentCtrlModel.hpp"

#include "SpidercamCtrl.hpp"
#include "SpidercamUtils.hpp"
#include "SpidercamExperimentStates.hpp"
#include "../../Utilities/Timers.hpp"
#include "../../Utilities/Utilities.hpp"

#include <cbdf/SpidercamSerializer.hpp>


class cSpidercamModel : public cExperimentControlModel
{
    Q_OBJECT

public:
	cSpidercamModel(QObject* parent = nullptr);
	~cSpidercamModel();

	/*
	 * Returns a string used as a descriptor of the experiment
	 * controller.
	 */
	char* descriptor() const override { return "spidercam"; };

	const spidercam::sPosition_1_t& currentPosition() const;

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    void configure(const nlohmann::json& jsonCfg) override;

	/*
	 * Is the system ready to run an experiment?
	*/
	bool systemReady() const override;

	/*
	 * Create a experiment state for the state machine
	 */
	cExperimentState* createState(const std::string& type) override;

	/**
	 * Started the loaded experiment.
	 *
	 * Returns true if the experiment was started, false otherwise.
	 */
	void startExperiment() override;

	/*
	 * Attach/Detach the serializer to the data file.
	 */
	void enableDataRecording(cBlockDataFileWriter& file) override;
	void disableDataRecording() override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader() override;
	void stopDataRecording() override;

	bool startCommunications() override;
	void stopCommunications() override;

signals:
    void limitsChanged(spidercam::sWorkingDimensions limits);
    void positionChanged(spidercam::sPosition_1_t pos);
	void busyChanged(bool busy);
	void movingChanged(bool moving);
	void batteryLevelChanged(float level_pct);
	void obstacleDistanceChanged(float distantance_mm);
	void inPositionStateChanged(bool in_position);
	void inScriptMode(bool in_script_mode);

/*
	bool mIpCameraConnected;
	bool mSafetyCircuitRestarted;
	bool mPlaceKnown;
	bool mBordersKnown;
	bool mFieldKnown;
	bool mSetPointEnabled;
	bool mCalibrated;
	bool mCableLengthAdjustmentRequired;
	bool mNearBorder;
	bool mAtCalibrationPosition;
	bool mAtCorrectionPosition;
*/

protected slots:
	void onConnectionStateChange(bool connected);

protected:
	void update() override;

	void updateState();
	void updateObstacleDistance();

protected:

	spidercam::sPosition_1_t mCurrentPosition;


	comparator<int, spidercam::window_compare<int, 2>> mBatteryLevel_pct;

	bool mDollyConnected;
	bool mConsoleConnected;
	bool mActivated;
	bool mDollyPositionKnown;
	bool mConsoleEnabled;
	bool mInInteractiveMode;

	edge_detect<bool>	mBusy;
	edge_detect<bool>	mInError;
	edge_detect<bool>	mDone;
	edge_detect<bool>	mMoving;
	edge_detect<bool>	mObstacleLessThan2000mm;
	edge_detect<bool>	mObstacleLessThan1500mm;
	edge_detect<bool>	mObstacleLessThan1000mm;
	edge_detect<bool>	mObstacleLessThan500mm;
	edge_detect<bool>	mInPosition;
	edge_detect<bool>   mInScriptMode;

	cIntervalTimer	mTimer;

	double mPositionTolerance_mm;

private:
    cSpidercamController mController;
	cSpidercamSerializer mSerializer;
};


inline const spidercam::sPosition_1_t& cSpidercamModel::currentPosition() const
{
	return mCurrentPosition;
}



