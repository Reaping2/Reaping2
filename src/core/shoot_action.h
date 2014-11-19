#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
protected:
    ShootAction( int32_t Id );
    friend class Factory<Action>;
    virtual void Update( double Seconds );
};
class ShootAltAction : public Action
{
protected:
    ShootAltAction( int32_t Id );
    friend class Factory<Action>;
    virtual void Update( double Seconds );
};
#endif//INCLUDED_SHOOT_ACTION_H
