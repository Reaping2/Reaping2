#ifndef INCLUDED_CORE_CONTROLLER_H
#define INCLUDED_CORE_CONTROLLER_H

class Actor;
class Controller
{
public:
    Controller();
    virtual ~Controller();
    virtual void SetActor( Actor* Obj );
    virtual void Update( double Seconds );
protected:
    Actor* mActor;
};

#endif//INCLUDED_CORE_CONTROLLER_H
