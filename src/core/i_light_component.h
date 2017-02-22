#ifndef INCLUDED_CORE_I_LIGHT_COMPONENT_H
#define INCLUDED_CORE_I_LIGHT_COMPONENT_H

#include "component.h"

class ILightComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ILightComponent)
    virtual void SetRadius(double radius)=0;
    virtual double GetRadius()const=0;
    virtual void SetAperture(double radius)=0;
    virtual double GetAperture()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ILightComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_LIGHT_COMPONENT_H

//command:  "../../build-reap-release/bin/relwithdebinfo/classgenerator" -g "component" -c "light_component" -m "double-radius"
