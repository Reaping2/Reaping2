#ifndef INCLUDED_CORE_BODY_IDLE_ACTION_H
#define INCLUDED_CORE_BODY_IDLE_ACTION_H

class BodyIdleAction : public Action
{
protected:
	BodyIdleAction(int32_t Id);
	virtual void Update(double Seconds)  override;
	friend class Factory<Action>;
};

#endif//INCLUDED_CORE_BODY_IDLE_ACTION_H
