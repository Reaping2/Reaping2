#ifndef INCLUDED_CORE_BODY_MOVE_ACTION_H
#define INCLUDED_CORE_BODY_MOVE_ACTION_H

class BodyMoveAction : public Action
{
protected:
	BodyMoveAction();
	virtual void Update(Actor& Actor,double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_BODY_MOVE_ACTION_H
