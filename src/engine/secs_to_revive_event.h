#ifndef INCLUDED_ENGINE_SECS_TO_REVIVE_EVENT_H
#define INCLUDED_ENGINE_SECS_TO_REVIVE_EVENT_H

#include "platform/event.h"

namespace engine {

struct SecsToReviveEvent : public platform::Event
{
    int32_t mClientId;
    double mSecsToRevive;
    SecsToReviveEvent( int32_t clientId, double secsToRevive )
        : mClientId( clientId ), mSecsToRevive( secsToRevive ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SECS_TO_REVIVE_EVENT_H

//command:  "classgenerator" -g "event" -c "secs_to_revive_event" -m "int32_t-clientId double-secsToRevive"
