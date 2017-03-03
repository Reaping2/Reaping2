#ifndef INCLUDED_CORE_ACTOR_EVENT_H
#define INCLUDED_CORE_ACTOR_EVENT_H
#include "actor.h"
#include "opt.h"

struct ActorEvent : public Event
{
    enum State : int32_t
    {
        Added = 0,
        Died,
        Removed
    };
    Opt<Actor> mActor;
    State mState;
    ActorEvent( Opt<Actor> actor, State state ): mActor( actor ), mState( state ) {}
};

struct SceneLoadEvent : public Event
{

};

#endif//INCLUDED_CORE_ACTOR_EVENT_H
