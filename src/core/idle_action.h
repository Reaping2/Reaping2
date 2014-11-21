#ifndef INCLUDED_CORE_IDLE_ACTION_H
#define INCLUDED_CORE_IDLE_ACTION_H

class IdleAction : public Action
{
protected:
    IdleAction( int32_t Id );
    virtual void Update( double Seconds );
    friend class platform::Factory<Action>;
};

#endif//INCLUDED_CORE_IDLE_ACTION_H
