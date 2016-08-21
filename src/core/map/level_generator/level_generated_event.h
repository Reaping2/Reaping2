#ifndef INCLUDED_MAP_LEVEL_GENERATED_EVENT_H
#define INCLUDED_MAP_LEVEL_GENERATED_EVENT_H

#include "platform/event.h"

namespace map {

struct LevelGeneratedEvent : public platform::Event
{
    enum State
    {
        TerrainGenerated,
        ActorsSpawned
    } mState;
    LevelGeneratedEvent(State state)
        :mState(state){}
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "level_generated_event" -n "map" -m "State-state"
