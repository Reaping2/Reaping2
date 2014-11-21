#ifndef INCLUDED_CORE_DEATH_ACTION_H
#define INCLUDED_CORE_DEATH_ACTION_H

class DeathAction : public Action
{
    friend class platform::Factory<Action>;
    DeathAction( int32_t Id );
    void Update( double Seconds );
};

#endif//INCLUDED_CORE_DEATH_ACTION_H
