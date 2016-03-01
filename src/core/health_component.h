#ifndef INCLUDED_CORE_HEALTH_COMPONENT_H
#define INCLUDED_CORE_HEALTH_COMPONENT_H

#include "core/i_health_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

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
    virtual void SetLastDamageOwnerGUID(int32_t lastDamageOwnerGUID);
    virtual int32_t GetLastDamageOwnerGUID()const;

    HealthComponent();
    friend class ComponentFactory;

    int32_t mHP;
    int32_t mDamage;
    int32_t mHeal;
    bool mAlive;
    double mTimeOfDeath;
    Buffable<int32_t> mMaxHP;
    int32_t mLastDamageOwnerGUID;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void HealthComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IHealthComponent>(*this);
    ar & mHP;
    ar & mDamage;
    ar & mHeal;
    ar & mAlive;
    ar & mTimeOfDeath;
    ar & mMaxHP;
    ar & mLastDamageOwnerGUID;
}

class HealthComponentLoader: public ComponentLoader<HealthComponent>
{
    virtual void BindValues();
public:
    HealthComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2(HealthComponent, HealthComponent,"health_component");
#endif//INCLUDED_CORE_HEALTH_COMPONENT_H