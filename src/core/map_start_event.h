#ifndef INCLUDED_CORE_MAP_START_EVENT_H
#define INCLUDED_CORE_MAP_START_EVENT_H

#include "platform/event.h"

namespace core {

struct MapStartEvent : public platform::Event
{
    enum State : int32_t
    {
        Started,
        PreActorsSpawned,
        ActorsSpawned,
        Ready
    } mState;
    MapStartEvent(State state):mState(state) {}
};

} // namespace core

#endif//INCLUDED_CORE_MAP_START_EVENT_H
