#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
	ShootAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds) ;
	friend class Factory<Action,Actor>;
};
class ShootAltAction : public Action
{
protected:
	ShootAltAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds) ;
	friend class Factory<Action,Actor>;
};
#endif//INCLUDED_SHOOT_ACTION_H
