#ifndef INCLUDED_CORE_I_COLLISION_COMPONENT_H
#define INCLUDED_CORE_I_COLLISION_COMPONENT_H
#include "component.h"
#include "core/collision_class.h"

class ICollisionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ICollisionComponent)
    virtual CollisionClass::Type GetCollisionClass()const=0;
    virtual void SetCollisionClass( CollisionClass::Type CollisionClassType )=0;
    virtual double GetRadius()const=0;
    virtual void SetRadius( double Radius)=0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ICollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_COLLISION_COMPONENT_H