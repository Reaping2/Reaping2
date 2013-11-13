#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
	ShootAction();
	virtual void Update(Actor& Actor,double Seconds) ;
	friend class ActionRepo;
};
class ShootAltAction : public Action
{
protected:
	ShootAltAction();
	virtual void Update(Actor& Actor,double Seconds) ;
	friend class ActionRepo;
};
#endif//INCLUDED_SHOOT_ACTION_H
