#ifndef INCLUDED_CORE_DAMAGE_ACTION_H
#define INCLUDED_CORE_DAMAGE_ACTION_H
#include "core\action.h"
#include "platform\factory.h"

class DamageAction : public Action
{
public:
    void SetDamage( int32_t Damage );
protected:
    friend class platform::Factory<Action>;
    DamageAction( int32_t Id );
    bool Activate();
    int32_t mDamage;
};

#endif//INCLUDED_CORE_DAMAGE_ACTION_H
