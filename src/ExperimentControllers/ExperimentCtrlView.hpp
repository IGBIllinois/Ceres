
#pragma once

//#include <QWidget>
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

protected:
	cExperimentControlView() = default;
};