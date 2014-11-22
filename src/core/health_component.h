#ifndef INCLUDED_CORE_HEALTH_COMPONENT_H
#define INCLUDED_CORE_HEALTH_COMPONENT_H

#include "i_health_component.h"

class Actor;
class HealthComponent : public IHealthComponent
{
public:
    virtual void Update( double Seconds );
    virtual int32_t const& GetHP()const;
    virtual void SetHP( int32_t Hp );
    virtual bool IsAlive()const;
    virtual double GetTimeOfDeath()const;
    virtual void SetActor( Actor* Obj );
    virtual bool NeedDelete()const;
protected:
    HealthComponent();
    friend class ComponentFactory;

    int32_t mHP;
    bool mAlive;
    double mTimeOfDeath;
    Actor* mActor;
    bool mNeedDelete;
};

#endif//INCLUDED_CORE_HEALTH_COMPONENT_H