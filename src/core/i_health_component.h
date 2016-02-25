#ifndef INCLUDED_CORE_I_HEALTH_COMPONENT_H
#define INCLUDED_CORE_I_HEALTH_COMPONENT_H
#include "component.h"
#include "buffable.h"
#include <boost/serialization/export.hpp>

class IHealthComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IHealthComponent)
    virtual void Update( double Seconds )=0;
    virtual int32_t const& GetHP()const=0;
    virtual void SetHP( int32_t Hp )=0;
    virtual bool IsAlive()const=0;
    virtual void SetAlive( bool alive )=0;
    virtual void TakeDamage( int32_t damage )=0;
    virtual int32_t GetDamage()=0;
    virtual void ResetDamage()=0;
    virtual void TakeHeal( int32_t damage )=0;
    virtual int32_t GetHeal()=0;
    virtual void ResetHeal()=0;
    virtual double GetTimeOfDeath()const=0;
    virtual void SetTimeOfDeath(double timeOfDeath)=0;
    virtual Buffable<int32_t>& GetMaxHP()=0;
    virtual void SetLastDamageOwnerGUID(int32_t lastDamageOwnerGUID)=0;
    virtual int32_t GetLastDamageOwnerGUID()const=0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IHealthComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}


BOOST_CLASS_EXPORT_KEY2(IHealthComponent,"i_health_component");
#endif//INCLUDED_CORE_I_HEALTH_COMPONENT_H