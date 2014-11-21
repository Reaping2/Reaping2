#ifndef INCLUDED_MOVE_ACTION_H
#define INCLUDED_MOVE_ACTION_H

class MoveAction : public Action
{
public:
    virtual void SetActor( Actor* Obj );

protected:
    MoveAction( int32_t Id );
    virtual void Update( double Seconds );
    friend class platform::Factory<Action>;
};

#endif//INCLUDED_MOVE_ACTION_H
