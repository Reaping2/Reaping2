#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

#include "core/i_collision_component.h"

class Action;
class Item;
typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > AllActorMemberHook_t;
class Actor : public AutoId, public ComponentHolder
{
public:
    AllActorMemberHook_t mAllActorHook;
    union Field_t
    {
        int32_t i;
        double d;
    };

    typedef boost::ptr_map<int32_t, Action> ActionList_t;

protected:
    enum
    {
        DAMAGE,

        RADIUS,
        COLLISION_CLASS,
        GUID,           // todo: MakeGuid()
        NUM_FIELDS
    };

    ActionRepo& mActionFactory;
    ActionList_t mActions;

    Field_t mFields[NUM_FIELDS];

    bool CanAddAction( int32_t Id )const;
public:
    Actor( std::string const& Name );
    virtual ~Actor() = 0;
    virtual void Collide( Actor& Other );
    virtual void ClipScene();
    virtual void Update( double Seconds );

    double GetRadius()const
    {
        return mFields[RADIUS].d;
    }

    ActionList_t const& GetActions()const
    {
        return mActions;
    }




    void AddAction( int32_t Id );
    void AddAction( std::auto_ptr<Action> Act );
    bool HasAction( int32_t Id )const;
    void DropAction( int32_t Id );

    int32_t GetGUID()const
    {
        return mFields[GUID].i;
    }
    CollisionClass::Type GetCC()const
    {
        return CollisionClass::Type( mFields[COLLISION_CLASS].i );
    }
    void SetCC( CollisionClass::Type CCType )
    {
        mFields[COLLISION_CLASS].i = CCType;
    }
};

typedef boost::intrusive::member_hook< Actor, AllActorMemberHook_t, &Actor::mAllActorHook> AllActorOption_t;
typedef boost::intrusive::list<Actor, AllActorOption_t, boost::intrusive::constant_time_size<false> > ActorList_t;


#endif//INCLUDED_CORE_ACTOR_H
