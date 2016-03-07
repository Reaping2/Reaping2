#ifndef INCLUDED_ENGINE_SCORE_EVENT_H
#define INCLUDED_ENGINE_SCORE_EVENT_H

#include "platform/event.h"

namespace engine {

struct ScoreEvent : public platform::Event
{
    int32_t mActorGUID;
    ScoreEvent( int32_t actorGUID )
        : mActorGUID( actorGUID ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SCORE_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "score_event" -m "int32_t-actorGUID"
