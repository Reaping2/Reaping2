#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
	ShootAction();
	virtual void Update(Actor& Actor,double Seconds) const {}
	friend class ActionHolder;
};

#endif//INCLUDED_SHOOT_ACTION_H
