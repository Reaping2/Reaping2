#ifndef INCLUDED_CORE_RANDOM_CONTROLLER_H
#define INCLUDED_CORE_RANDOM_CONTROLLER_H

class RandomController : public Controller
{
public:
    RandomController();
    virtual void Update( double Seconds );
    virtual void SetActor( Actor* Obj );
private:
    double mCounter;
    double mHeadingModifier;
};

#endif//INCLUDED_CORE_RANDOM_CONTROLLER_H
