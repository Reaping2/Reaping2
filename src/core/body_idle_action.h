#ifndef INCLUDED_CORE_BODY_IDLE_ACTION_H
#define INCLUDED_CORE_BODY_IDLE_ACTION_H

class BodyIdleAction : public Action
{
protected:
	BodyIdleAction();
	virtual void Update(Actor& Actor,double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_BODY_IDLE_ACTION_H
