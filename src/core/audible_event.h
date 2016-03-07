#ifndef INCLUDED_CORE_AUDIBLE_EVENT_H
#define INCLUDED_CORE_AUDIBLE_EVENT_H

#include "platform/event.h"

namespace core {
struct AudibleEvent : public Event
{
    int32_t const mActorGUID;
    int32_t const mId;
    bool const mIsOneShot;
    bool const mIsAddition;
    AudibleEvent( int32_t ActorId, int32_t Id, bool IsOneShot, bool IsAddition )
        : mActorGUID( ActorId )
        , mId( Id )
        , mIsOneShot( IsOneShot )
        , mIsAddition( IsAddition ) {}
};
}

#endif//INCLUDED_CORE_AUDIBLE_EVENT_H
