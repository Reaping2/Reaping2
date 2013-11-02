#ifndef INCLUDED_MOVE_ACTION_H
#define INCLUDED_MOVE_ACTION_H

class MoveAction : public Action
{
protected:
	MoveAction(){}
	virtual void Update(double Seconds, Actor& Actor){}
	friend class ActionHolder;
};

#endif//INCLUDED_MOVE_ACTION_H
