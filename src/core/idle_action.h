#ifndef INCLUDED_CORE_IDLE_ACTION_H
#define INCLUDED_CORE_IDLE_ACTION_H

class IdleAction : public Action
{
protected:
	IdleAction();
	virtual void Update(Actor& Actor,double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_IDLE_ACTION_H
