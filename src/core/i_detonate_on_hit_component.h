#ifndef INCLUDED_CORE_I_DETONATE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_I_DETONATE_ON_HIT_COMPONENT_H

#include "component.h"

class IDetonateOnHitComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IDetonateOnHitComponent)
    virtual void SetMaterial(int32_t material)=0;
    virtual int32_t GetMaterial()const=0;
    virtual void SetAddRadius(double addRadius)=0;
    virtual double GetAddRadius()const=0;
    virtual void SetRemoveOnHit(bool removeOnHit)=0;
    virtual bool IsRemoveOnHit()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IDetonateOnHitComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_DETONATE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_detonate_on_hit_component" -m "int32_t-material double-addRadius bool-removeOnHit"
