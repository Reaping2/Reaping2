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
		Num_Classes,
	};
};
class Action;
class Actor;
typedef boost::intrusive::list<Actor, boost::intrusive::constant_time_size<false> > ActorList;
typedef boost::intrusive::list_member_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > AllActorMemberHook_t;
class Actor : public AutoId, public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> >
{
public:
	AllActorMemberHook_t mAllActorHook;
	union Field_t
	{
		int32_t i;
		double d;
	};

	// kikurni kulon fileba?
	class ActionDesc_t
	{
	private:
		friend class Actor;
		Action const* mAction;
		Field_t mId;
		Field_t mState;
		ActionDesc_t(Action const* A,double S=0.);
	public:
		int32_t GetId()const{return mId.i;}
		double GetState()const{return mState.d;}
		void SetState(double S){mState.d=S;}
		Action const* GetAction()const{return mAction;}
	};

	typedef std::list<ActionDesc_t> ActionDescList_t;
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
		ACTION_ID,
		ACTION_STATE,
		NUM_FIELDS,
	};

	Field_t mFields[NUM_FIELDS];
	ActionDescList_t mActions;
	std::auto_ptr<Controller> mController;

	void UpdateProjections()
	{
		const double spd=GetSpeed();
		const double h=GetHeading();
		const double c=cos(h);
		const double s=sin(h);
		mFields[SPEED_X].d=c*spd;
		mFields[SPEED_Y].d=s*spd;
	}
public:
	Actor(std::string const& Name);
	virtual ~Actor(){}
	virtual void Collide(double Seconds, ActorList& Actors);
	virtual void Update(double Seconds);

	void SetController(std::auto_ptr<Controller> Control);

	double GetX()const{return mFields[X].d;}
	double GetY()const{return mFields[Y].d;}
	double GetRadius()const{return mFields[RADIUS].d;}
	double GetSpeed()const{return mFields[SPEED].d;}
	double GetSpeedX()const{return mFields[SPEED_X].d;}
	double GetSpeedY()const{return mFields[SPEED_Y].d;}
	double GetHeading()const{return mFields[HEADING].d;}
	double GetOrientation()const{return mFields[ORIENTATION].d;}
	ActionDescList_t const& GetActions()const{return mActions;}
	ActionDesc_t* GetActionDesc(int32_t Id);
	void AddAction(Action const& Act);
	void DropAction(Action const& Act);
	int32_t GetHP()const{return mFields[HP].i;}
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
	void SetAction(int32_t Action)
	{
		mFields[ACTION_ID].i=Action;
	}
	void SetActionState(double ActionState)
	{
		mFields[ACTION_STATE].d=ActionState;
	}

	void ClearActions();
/*	void SetActionIdPos(ActionRepo::ActionType ActionId, int32_t Position, bool Activate=true);
	void SetActionStatePos(int32_t Position, int32_t State=0xFF);
	bool HasAction(ActionRepo::ActionType ActionId, int32_t& Position);*/
};

typedef boost::intrusive::member_hook< Actor, AllActorMemberHook_t, &Actor::mAllActorHook> AllActorOption_t;
typedef boost::intrusive::list<Actor, AllActorOption_t, boost::intrusive::constant_time_size<false> > AllActorInSceneList;


#endif//INCLUDED_CORE_ACTOR_H