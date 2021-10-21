
#pragma once

//#include "../DataFile.hpp"

#include <QObject>
#include <nlohmann/json.hpp>

class cBlockDataFile;

class cExperimentControlModel : public QObject
{
    Q_OBJECT

public:
	virtual ~cExperimentControlModel() = default;

    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    virtual void configure(const nlohmann::json& jsonCfg) = 0;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader(cBlockDataFile& file) = 0;

signals:
    void statusMessage(QString msg);

protected:
    cExperimentControlModel() = default;


};