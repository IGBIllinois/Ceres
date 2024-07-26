
#pragma once

#include "DataThread.hpp"

// Forward Declarations
class cExperimentControlModel;

class cPlannerDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cPlannerDataThread();
    virtual ~cPlannerDataThread();

protected:
    void updateAll() override;

    bool startCommunications() override;
    bool stopCommunications() override;

public:
    cExperimentControlModel* mpController;
};

