
#pragma once

#include "ExperimentCtrlModel.hpp"

#include "SpidercamCtrl.hpp"

class cSpidercamModel : public cExperimentControlModel
{
    Q_OBJECT

public:
	cSpidercamModel();
	~cSpidercamModel();


    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    void configure(const nlohmann::json& jsonCfg) override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader(cBlockDataFile& file) override;


signals:
    void updatePosition();

protected:

private:
    cSpidercamController mController;
};

