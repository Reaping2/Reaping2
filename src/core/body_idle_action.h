#ifndef INCLUDED_CORE_BODY_IDLE_ACTION_H
#define INCLUDED_CORE_BODY_IDLE_ACTION_H

class BodyIdleAction : public Action
{
protected:
	BodyIdleAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_BODY_IDLE_ACTION_H
