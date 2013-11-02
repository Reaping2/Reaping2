#ifndef INCLUDED_ACTION_H
#define INCLUDED_ACTION_H

class Action
{
public:
	virtual ~Action(){}
protected:
	Action(){}
	virtual void Update(double Seconds, Actor& Actor){}
	friend class ActionHolder;
};

#endif//INCLUDED_ACTION_H