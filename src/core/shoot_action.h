#ifndef INCLUDED_SHOOT_ACTION_H
#define INCLUDED_SHOOT_ACTION_H

class ShootAction : public Action
{
    ShootAction( int32_t Id );
    friend class Factory<Action>;
};
class ShootAltAction : public Action
{
    ShootAltAction( int32_t Id );
    friend class Factory<Action>;
};
#endif//INCLUDED_SHOOT_ACTION_H
