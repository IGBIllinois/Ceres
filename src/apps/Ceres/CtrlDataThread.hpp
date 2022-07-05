
#pragma once

#include "DataThread.hpp"

// Forward Declarations
class cExperimentControlModel;

class cCtrlDataThread : public cDataThread
{
    Q_OBJECT

public:
    explicit cCtrlDataThread();
    virtual ~cCtrlDataThread();

protected:
    void updateAll() override;

    bool startCommunications() override;
    bool stopCommunications() override;

public:
    cExperimentControlModel* mpController;
};

