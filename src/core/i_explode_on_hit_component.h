#ifndef INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H

#include "component.h"
#include "i_explode.h"
#include "platform/export.h"

class IExplodeOnHitComponent : public Component, public IExplode
{
public:
    IExplodeOnHitComponent();
    DEFINE_COMPONENT_BASE(IExplodeOnHitComponent)
    virtual void SetDetonatorMaterial(int32_t detonatorMaterial)=0;
    virtual int32_t GetDetonatorMaterial()const=0;
    virtual void SetAddActorRadius(double addRadius)=0;
    virtual double GetAddRadius()const=0;
    virtual void SetExploded(bool exploded)=0;
    virtual bool IsExploded()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IExplodeOnHitComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
    ar & boost::serialization::base_object<IExplode>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(IExplodeOnHitComponent, IExplodeOnHitComponent,"i_explode_on_hit_component");
#endif//INCLUDED_CORE_I_EXPLODE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_explode_on_hit_component" -m "int32_t-detonatorMaterial double-addRadius bool-exploded"
