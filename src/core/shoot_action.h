#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
	ShootAction(int32_t Id);
	virtual void Update(double Seconds) ;
	friend class Factory<Action>;
};
class ShootAltAction : public Action
{
protected:
	ShootAltAction(int32_t Id);
	virtual void Update(double Seconds) ;
	friend class Factory<Action>;
};
#endif//INCLUDED_SHOOT_ACTION_H
