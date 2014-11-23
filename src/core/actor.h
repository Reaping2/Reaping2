#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

#include "core/i_collision_component.h"
#include "core/action_holder.h"
class Action;
class Item;



typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > AllActorMemberHook_t;
class Actor : public AutoId, public ComponentHolder, public ActionHolder
{
public:
    AllActorMemberHook_t mAllActorHook;
protected:
    int32_t mGUID;
public:
    //TODO: only one addaction
    void AddAction( int32_t Id );
    virtual void AddAction( std::auto_ptr<Action> Act );
    Actor( std::string const& Name );
    virtual ~Actor() = 0;

    int32_t GetGUID()const
    {
        return mGUID;
    }

    virtual void Update( double Seconds );
};

typedef boost::intrusive::member_hook< Actor, AllActorMemberHook_t, &Actor::mAllActorHook> AllActorOption_t;
typedef boost::intrusive::list<Actor, AllActorOption_t, boost::intrusive::constant_time_size<false> > ActorList_t;


#endif//INCLUDED_CORE_ACTOR_H
