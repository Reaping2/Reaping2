#ifndef INCLUDED_ENGINE_SOLDIER_CREATED_EVENT_H
#define INCLUDED_ENGINE_SOLDIER_CREATED_EVENT_H

#include "platform/event.h"
#include "core/program_state.h"
#include "core/actor.h"

namespace engine {

struct SoldierCreatedEvent : public platform::Event
{
    enum State
    {
        Raw,
        PropertiesSet,
        ComponentsSet,
        Finished
    } mState;
    core::ClientData& mClientData;
    Opt<Actor> mActor;
    SoldierCreatedEvent( core::ClientData& clientData, Opt<Actor> actor, State state = State::Raw )
        : mState(state), mClientData( clientData ), mActor( actor ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_CREATED_EVENT_H
