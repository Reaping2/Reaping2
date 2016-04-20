#ifndef INCLUDED_CORE_I_ROTATE_COMPONENT_H
#define INCLUDED_CORE_I_ROTATE_COMPONENT_H

#include "component.h"

class IRotateComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IRotateComponent)
    virtual void SetSpeed(double speed)=0;
    virtual double GetSpeed()const=0;
    virtual void SetRotating(bool rotating)=0;
    virtual bool IsRotating()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IRotateComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_ROTATE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "rotate_component" -m "double-speed bool-rotating"
