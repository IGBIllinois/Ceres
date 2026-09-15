
#pragma once

#include "ExperimentStateMachine.hpp"
#include "ExperimentTypes.hpp"

#include <QObject>
#include <nlohmann/json.hpp>
#include <vector>

// Forward Declarations
class cExperimentState;
class cBlockDataFileWriter;


class cExperimentControlModel : public cExperimentStateMachine
{
    Q_OBJECT

public:
	virtual ~cExperimentControlModel() = default;

    /*
     * Returns a string used as a descriptor of the experiment
     * controller.
     */
    virtual const char* descriptor() const = 0;

    /*
     * Apply any configuration parameters to the experiment
     * controller model.
     */
    virtual void configure(const nlohmann::json& jsonCfg) = 0;

    /*
     * Is the system ready to run an experiment?
    */
    virtual bool systemReady() const = 0;

    /*
     * Attach/Detach the serializer to the data file.
     */
    virtual void enableDataRecording(cBlockDataFileWriter& file) = 0;
    virtual void disableDataRecording() = 0;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    virtual void writeDataHeader() = 0;
    virtual void writeDataFooter() {};

    /*
     * Sets the non-owning data file pointer to null to
     * stop the writing of sensor data.
     */
    virtual void stopDataRecording() = 0;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    virtual bool startCommunications() = 0;
    virtual void stopCommunications() = 0;

signals:
    void updateControllerConnection(bool connected);

    void statusMessage(QString msg) const;
    void infoMessage(QString title, QString msg) const;
    void warningMessage(QString title, QString msg) const;
    void errorMessage(QString title, QString msg) const;

    void measurementName(QString name) const;

public:
    virtual void update() = 0;

protected:
    cExperimentControlModel(QObject* parent = nullptr);

    void handleStateErrorMessage(const QString& msg) override;

    void emitStatusMessage(const QString& msg) override;
    void emitMeasurementName(const QString& name) override;
};