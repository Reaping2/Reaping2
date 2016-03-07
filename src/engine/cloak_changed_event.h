#ifndef INCLUDED_ENGINE_CLOAK_CHANGED_EVENT_H
#define INCLUDED_ENGINE_CLOAK_CHANGED_EVENT_H

#include "platform/event.h"

namespace engine {

struct CloakChangedEvent : public platform::Event
{
    int32_t mActorGUID;
    bool mActivated;
    CloakChangedEvent( int32_t actorGUID, bool activated )
        : mActorGUID( actorGUID ), mActivated( activated ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CLOAK_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "cloak_changed_event" -m "int32_t-actorGUID bool-activated"
