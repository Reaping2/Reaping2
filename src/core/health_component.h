#ifndef INCLUDED_CORE_HEALTH_COMPONENT_H
#define INCLUDED_CORE_HEALTH_COMPONENT_H

#include "i_health_component.h"
#include "component_loader.h"

class Actor;
class HealthComponent : public IHealthComponent
{
public:
    virtual void Update( double Seconds );
    virtual int32_t const& GetHP()const;
    virtual void SetHP( int32_t Hp );
    virtual bool IsAlive()const;
    virtual double GetTimeOfDeath()const;
    virtual bool NeedDelete()const;
protected:
    HealthComponent();
    friend class ComponentFactory;

    int32_t mHP;
    bool mAlive;
    double mTimeOfDeath;
    bool mNeedDelete;
};

class HealthComponentLoader:ComponentLoader<HealthComponent>
{
    virtual void LoadValues(Json::Value& setters);
protected:
    HealthComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_HEALTH_COMPONENT_H