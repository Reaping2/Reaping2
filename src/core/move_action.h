#ifndef INCLUDED_MOVE_ACTION_H
#define INCLUDED_MOVE_ACTION_H

class MoveAction : public Action
{
protected:
	MoveAction();
	virtual void Update(Actor& Actor,double Seconds) const override;
	friend class ActionHolder;
};

#endif//INCLUDED_MOVE_ACTION_H
