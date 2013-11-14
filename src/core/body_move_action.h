#ifndef INCLUDED_CORE_BODY_MOVE_ACTION_H
#define INCLUDED_CORE_BODY_MOVE_ACTION_H

class BodyMoveAction : public Action
{
protected:
	BodyMoveAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_BODY_MOVE_ACTION_H
