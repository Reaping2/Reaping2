#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

struct CollisionClass
{
    enum Type
    {
        No_Collision,
        Projectile,
        Creep,
        Mine,
        Player,
        Wall,
        Pickup,
        Num_Classes,
    };
};
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

    enum
    {
        HP_DEAD = -1,
    };
protected:
    enum
    {
        HP,
        DAMAGE,

        RADIUS,
        COLLISION_CLASS,
        GUID,           // todo: MakeGuid()
        TIME_OF_DEATH,
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
    virtual void UpdateLifetime();
    virtual void TakeDamage( int32_t Damage );
    virtual void OnDeath() {}
    bool IsAlive()const;

    double GetRadius()const
    {
        return mFields[RADIUS].d;
    }

    ActionList_t const& GetActions()const
    {
        return mActions;
    }




    void AddAction( int32_t Id );
    bool HasAction( int32_t Id )const;
    void DropAction( int32_t Id );
    int32_t const& GetHP()const
    {
        return mFields[HP].i;
    }
    void SetHP( int32_t Hp )
    {
        mFields[HP].i = Hp;
    }
    double GetTimeOfDeath()const
    {
        return mFields[TIME_OF_DEATH].d;
    }
    int32_t GetGUID()const
    {
        return mFields[GUID].i;
    }
    CollisionClass::Type GetCC()const
    {
        return CollisionClass::Type( mFields[COLLISION_CLASS].i );
    }
};

typedef boost::intrusive::member_hook< Actor, AllActorMemberHook_t, &Actor::mAllActorHook> AllActorOption_t;
typedef boost::intrusive::list<Actor, AllActorOption_t, boost::intrusive::constant_time_size<false> > ActorList_t;

struct DamageTakenEvent : public Event
{
    glm::vec2 const Pos;
    DamageTakenEvent( double x, double y ): Pos( x, y ) {}
    DamageTakenEvent( glm::vec2 const& p ): Pos( p ) {}
};

#endif//INCLUDED_CORE_ACTOR_H
