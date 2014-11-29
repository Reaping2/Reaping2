#ifndef INCLUDED_MOVE_ACTION_H
#define INCLUDED_MOVE_ACTION_H
#include "core/action.h"

class MoveAction : public Action
{
protected:
    MoveAction( int32_t Id );
    virtual void Update( double Seconds );
    friend class platform::Factory<Action>;
};

#endif//INCLUDED_MOVE_ACTION_H
