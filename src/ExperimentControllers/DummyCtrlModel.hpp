
#pragma once

#include "ExperimentCtrlModel.hpp"

class cDummyControlModel : public cExperimentControlModel
{
    Q_OBJECT

public:
	cDummyControlModel(QObject* parent = nullptr);
	~cDummyControlModel();

	/*
	 * Returns a string used as a descriptor of the experiment
	 * controller.
	 */
	const char* descriptor() const override { return "dummy"; };

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    void configure(const nlohmann::json& jsonCfg) override;

	/*
	 * Is the system ready to run an experiment?
	*/
	bool systemReady() const override;

	/**
	 * Started the loaded experiment.
	 *
	 * Returns true if the experiment was started, false otherwise.
	 */
	void startExperiment() override;

	/*
	 * Attach/Detach the serializer to the data file.
	 */
	virtual void enableDataRecording(cBlockDataFileWriter& file);
	virtual void disableDataRecording();

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader() override;
	void stopDataRecording() override;

	bool startCommunications() override;
	void stopCommunications() override;

protected:
	void update() override;
};

