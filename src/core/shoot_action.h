#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
	ShootAction();
	virtual void Update(double Seconds, Actor& Actor){}
		friend class ActionHolder;
};

#endif//INCLUDED_SHOOT_ACTION_H
