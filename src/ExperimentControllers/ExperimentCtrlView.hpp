
#pragma once

#include "ExperimentTypes.hpp"

#include <QAbstractScrollArea>
#include <nlohmann/json.hpp>


class cExperimentControlView : public QAbstractScrollArea
{
    Q_OBJECT

public:
	virtual ~cExperimentControlView() = default;

    /*
     * Apply any configuration parameters to the experiment
     * controller view.
     */
    virtual void configure(const nlohmann::json& jsonCfg);

signals:
    void statusMessage(QString msg);
    void infoMessage(QString title, QString msg);
    void warningMessage(QString title, QString msg);
    void errorMessage(QString title, QString msg);

public slots:
    virtual void experimentStateChanging(experiment::eState state);
    virtual void experimentStatusUpdating(QString msg);

protected:
	cExperimentControlView() = default;
};