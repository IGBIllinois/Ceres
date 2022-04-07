
#pragma once

#include <QObject>

namespace experiment
{
    enum class State : uint8_t
    {
        IDLE,
        LOADED,
        RUNNING,
        PAUSED,
        COMPLETED,
        TERMINATED,
        ERROR,
    };

    inline uint8_t to_int(State state)   { return static_cast<uint8_t>(state); }
    inline State to_state(uint8_t state) { return static_cast<State>(state); }

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
