
#pragma once

#include <QObject>

namespace experiment
{
    enum class eState : uint8_t
    {
        IDLE,
        LOADED,
        RUNNING,
        PAUSED,
        COMPLETED,
        TERMINATED,
        EXP_ERROR,
    };

    inline uint8_t to_int(eState state)   { return static_cast<uint8_t>(state); }
    inline eState to_state(uint8_t state) { return static_cast<eState>(state); }

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
