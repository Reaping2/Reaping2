#ifndef INCLUDED_CORE_BODY_MOVE_ACTION_H
#define INCLUDED_CORE_BODY_MOVE_ACTION_H

class BodyMoveAction : public Action
{
protected:
	BodyMoveAction(int32_t Id);
	virtual void Update(double Seconds)  override;
	friend class Factory<Action>;
};

#endif//INCLUDED_CORE_BODY_MOVE_ACTION_H
