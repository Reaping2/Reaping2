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
    typedef boost::ptr_list<Item> ItemList_t;

    enum
    {
        HP_DEAD = -1,
    };
protected:
    enum
    {
        HP,
        DAMAGE,

        ORIENTATION,    // merre nez
        RADIUS,
        HEADING,        // merre megy
        SPEED,
        SPEED_X,
        SPEED_Y,
        COLLISION_CLASS,
        GUID,           // todo: MakeGuid()
        TYPE_ID,
        TIME_OF_DEATH,
        COOLDOWN_REDUCTION,
        NUM_FIELDS
    };

    ActionRepo& mActionFactory;
    ActionList_t mActions;

    ItemRepo& mItemFactory;
    ItemList_t mItems;

    Field_t mFields[NUM_FIELDS];
    std::auto_ptr<Controller> mController;



    void UpdateProjections();
    bool CanAddAction( int32_t Id )const;
public:
    Actor( std::string const& Name );
    virtual ~Actor() = 0;
    virtual void Collide( Actor& Other );
    virtual void ClipScene();
    virtual void DoControlling( double Seconds );
    virtual void Update( double Seconds );
    virtual void UpdateLifetime();
    virtual void TakeDamage( int32_t Damage );
    virtual void OnDeath() {}
    bool IsAlive()const;

    void SetController( std::auto_ptr<Controller> Control );


    double GetRadius()const
    {
        return mFields[RADIUS].d;
    }
    double GetSpeed()const
    {
        return mFields[SPEED].d;
    }
    double GetSpeedX()const
    {
        return mFields[SPEED_X].d;
    }
    double GetSpeedY()const
    {
        return mFields[SPEED_Y].d;
    }
    double GetHeading()const
    {
        return mFields[HEADING].d;
    }
    double GetOrientation()const
    {
        return mFields[ORIENTATION].d;
    }
    double GetCooldownReduction()const
    {
        return mFields[COOLDOWN_REDUCTION].d;
    }
    ActionList_t const& GetActions()const
    {
        return mActions;
    }




	void AddAction( int32_t Id );
    bool HasAction( int32_t Id )const;
    ItemList_t const& GetItems()const;
    void AddItem( int32_t Id );
    void DropItemType( Item::ItemType Type );
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

    void SetSpeed( double Speed )
    {
        mFields[SPEED].d = Speed;
        UpdateProjections();
    }
    void SetHeading( double Heading )
    {
        mFields[HEADING].d = Heading;
        UpdateProjections();
    }
    void SetOrientation( double Ori )
    {
        mFields[ORIENTATION].d = Ori;
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
