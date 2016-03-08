#ifndef INCLUDED_ENGINE_FLAG_CREATED_EVENT_H
#define INCLUDED_ENGINE_FLAG_CREATED_EVENT_H

#include "platform/event.h"

namespace engine {

struct FlagCreatedEvent : public platform::Event
{
    Opt<Actor> mActor;
    FlagCreatedEvent( Opt<Actor> actor )
        : mActor( actor ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_FLAG_CREATED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "flag_created_event" -m "Opt<Actor>-actor"
