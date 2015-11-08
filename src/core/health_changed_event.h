#ifndef INCLUDED_ENGINE_HEALTH_CHANGED_EVENT_H
#define INCLUDED_ENGINE_HEALTH_CHANGED_EVENT_H

#include "platform/event.h"

namespace core {

struct HealthChangedEvent : public platform::Event
{
    Actor& mActor;
    HealthChangedEvent(Actor& actor)
        :mActor(actor){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_HEALTH_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "health_changed_event" -m "Actor&-actor"
