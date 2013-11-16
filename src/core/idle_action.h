#ifndef INCLUDED_CORE_IDLE_ACTION_H
#define INCLUDED_CORE_IDLE_ACTION_H

class IdleAction : public Action
{
protected:
	IdleAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds)  override;
	friend class Factory<Action,Actor>;
};

#endif//INCLUDED_CORE_IDLE_ACTION_H
