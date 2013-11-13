#ifndef INCLUDED_MOVE_ACTION_H
#define INCLUDED_MOVE_ACTION_H

class MoveAction : public Action
{
protected:
	MoveAction();
	virtual void Update(Actor& Actor,double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_MOVE_ACTION_H
