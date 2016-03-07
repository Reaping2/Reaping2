#ifndef INCLUDED_CORE_SHOT_EVENT_H
#define INCLUDED_CORE_SHOT_EVENT_H

#include "platform/event.h"
#include "platform/i_platform.h"

namespace core {

struct ShotEvent : public platform::Event
{
    int32_t mActorGUID;
    glm::vec2 mPosition;
    bool mIsAlt;
    ShotEvent( int32_t actorGUID, glm::vec2 position, bool isAlt )
        : mActorGUID( actorGUID ), mPosition( position ), mIsAlt( isAlt ) {}
};

} // namespace core

#endif//INCLUDED_CORE_SHOT_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "core" -c "shot_event" -m "int32_t-actorGUID glm::vec2-position bool-isAlt"
