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
};

#endif//INCLUDED_CORE_I_COLLISION_COMPONENT_H