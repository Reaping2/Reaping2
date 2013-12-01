#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

struct CollisionClass
{
	enum Type{
		No_Collision,
		Projectile,
		Creep,
		Mine,
		Player,
		Wall,
		Num_Classes,
	};
};
class Action;
class Item;
typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > AllActorMemberHook_t;
class Actor : public AutoId
{
public:
	AllActorMemberHook_t mAllActorHook;
	union Field_t
	{
		int32_t i;
		double d;
	};

		typedef boost::ptr_list<Action> ActionList_t;
		typedef boost::ptr_list<Item> ItemList_t;
protected:
	enum {
		HP,
		DAMAGE,
		X,
		Y,
		ORIENTATION,	// merre nez
		RADIUS,
		HEADING,		// merre megy
		SPEED,
		SPEED_X,
		SPEED_Y,
		COLLISION_CLASS,
		GUID,			// todo: MakeGuid()
		TYPE_ID,
		NUM_FIELDS
	};
	enum {
		HP_DEAD=-1,
	};

	ActionRepo& mActionFactory;
	ActionList_t mActions;
	Action * mDefaultAction;

	ItemRepo& mItemFactory;
	ItemList_t mItems;
	Item * mDefaultItem;


	Field_t mFields[NUM_FIELDS];
	std::auto_ptr<Controller> mController;

	void UpdateProjections();
public:
	Actor(std::string const& Name);
	virtual ~Actor();
	virtual void Collide(Actor& Other);
	virtual void ClipScene();
	virtual void DoControlling(double Seconds);
	virtual void Update(double Seconds);
	virtual void UpdateLifetime();
	virtual void TakeDamage(int32_t Damage);
	bool IsAlive()const;

	void SetController(std::auto_ptr<Controller> Control);

	Item& GetWeapon();
	Item const& GetWeapon() const;

	double const& GetX()const{return mFields[X].d;}
	double const& GetY()const{return mFields[Y].d;}
	double GetRadius()const{return mFields[RADIUS].d;}
	double GetSpeed()const{return mFields[SPEED].d;}
	double GetSpeedX()const{return mFields[SPEED_X].d;}
	double GetSpeedY()const{return mFields[SPEED_Y].d;}
	double GetHeading()const{return mFields[HEADING].d;}
	double GetOrientation()const{return mFields[ORIENTATION].d;}
	ActionList_t const& GetActions()const{return mActions;}
	Action& GetAction(int32_t Id);
	Action* AddAction(int32_t Id);
	Item& GetItem(int32_t Id);
	Item* AddItem(int32_t Id);
	void DropAction(int32_t Id);
	int32_t const& GetHP()const{return mFields[HP].i;}
	void SetHP(int32_t Hp){mFields[HP].i=Hp;}
	int32_t GetGUID()const{return mFields[GUID].i;}
	CollisionClass::Type GetCC()const{return CollisionClass::Type(mFields[COLLISION_CLASS].i);}
	void SetX(double x)
	{
		mFields[X].d=x;
	}
	void SetY(double y)
	{
		mFields[Y].d=y;
	}
	void SetSpeed(double Speed)
	{
		mFields[SPEED].d=Speed;
		UpdateProjections();
	}
	void SetHeading(double Heading)
	{
		mFields[HEADING].d=Heading;
		UpdateProjections();
	}
	void SetOrientation(double Ori)
	{
		mFields[ORIENTATION].d=Ori;
	}
};

typedef boost::intrusive::member_hook< Actor, AllActorMemberHook_t, &Actor::mAllActorHook> AllActorOption_t;
typedef boost::intrusive::list<Actor, AllActorOption_t, boost::intrusive::constant_time_size<false> > ActorList_t;

struct DamageTakenEvent : public Event
{
	glm::vec2 const Pos;
	DamageTakenEvent(double x, double y):Pos(x,y){}
	DamageTakenEvent(glm::vec2 const& p):Pos(p){}
};

#endif//INCLUDED_CORE_ACTOR_H
