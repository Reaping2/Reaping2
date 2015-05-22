#ifndef INCLUDED_CORE_HEALTH_COMPONENT_H
#define INCLUDED_CORE_HEALTH_COMPONENT_H

#include "core/i_health_component.h"
#include "core/property_loader.h"

class HealthComponent : public IHealthComponent
{
public:
    virtual void Update( double Seconds );
    virtual int32_t const& GetHP()const;
    virtual void SetHP( int32_t Hp );
    virtual bool IsAlive()const;
    virtual void SetAlive( bool alive );
    virtual void TakeDamage( int32_t damage );
    virtual int32_t GetDamage();
    virtual void ResetDamage();
    virtual void TakeHeal( int32_t heal );
    virtual int32_t GetHeal();
    virtual void ResetHeal();
    virtual double GetTimeOfDeath()const;
    virtual void SetTimeOfDeath( double timeOfDeath );
    virtual Buffable<int32_t>& GetMaxHP();
    void SetHPandMaxHP( int32_t Hp );

    HealthComponent();
    friend class ComponentFactory;

    int32_t mHP;
    int32_t mDamage;
    int32_t mHeal;
    bool mAlive;
    double mTimeOfDeath;
    Buffable<int32_t> mMaxHP;
};

class HealthComponentLoader: public ComponentLoader<HealthComponent>
{
    virtual void BindValues();
public:
    HealthComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_HEALTH_COMPONENT_H