
#pragma once

namespace experiment
{
    enum class State
    {
        IDLE,
        LOADED,
        RUNNING,
        PAUSED,
        COMPLETED,
        TERMINATED,
        ERROR,
    };

    enum class Event
    {
        NONE,
    };
}

class cExpEvent
{
public:
    cExpEvent(experiment::Event type) : mEvent(type) {}
    virtual ~cExpEvent() = default;

protected:
    experiment::Event mEvent;
};
