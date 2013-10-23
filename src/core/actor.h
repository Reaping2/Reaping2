#ifndef INCLUDED_CORE_ACTOR_H
#define INCLUDED_CORE_ACTOR_H

struct CollisionClass
{
	enum Type{
		Projectile,
		Creep,
		Mine,
		Player,
		Num_Classes,
	};
};
class Actor;
typedef boost::intrusive::list<Actor> ActorList;
class Actor : public boost::intrusive::list_base_hook<>
{
protected:
	enum {
		HP,
		DAMAGE,
		X,
		Y,
		RADIUS,
		HEADING,
		SPEED,
		SPEED_X,
		SPEED_Y,
		COLLISION_CLASS,
		NUM_FIELDS,
	};
	union field_t
	{
		int32_t i;
		double d;
	};
	field_t mFields[NUM_FIELDS];
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
	Actor();
	virtual ~Actor(){}
	virtual void Collide(double Seconds, ActorList& Actors);
	virtual void Update(double Seconds);

	double GetX()const{return mFields[X].d;}
	double GetY()const{return mFields[Y].d;}
	double GetRadius()const{return mFields[RADIUS].d;}
	double GetSpeed()const{return mFields[SPEED].d;}
	double GetHeading()const{return mFields[HEADING].d;}
	int32_t GetHP()const{return mFields[HP].i;}
	CollisionClass::Type GetCC()const{return CollisionClass::Type(mFields[COLLISION_CLASS].i);}

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
};


#endif//INCLUDED_CORE_ACTOR_H