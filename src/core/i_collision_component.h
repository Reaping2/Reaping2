#ifndef INCLUDED_CORE_I_COLLISION_COMPONENT_H
#define INCLUDED_CORE_I_COLLISION_COMPONENT_H
#include "component.h"


struct CollisionClass
{
    enum Type
    {
        No_Collision,
        Projectile,
        Creep,
        Mine,
        Player,
        Wall,
        Pickup,
        Num_Classes,
    };
};

class ICollisionComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ICollisionComponent)
    virtual CollisionClass::Type GetCollisionClass()const=0;
    virtual void SetCollisionClass( CollisionClass::Type CollisionClassType )=0;
    virtual void Collide( Actor& Other )=0;
    virtual void ClipScene()=0; //TODO: i don't really like this concept. This might be simply just a collision with scene edges, later maybe.
    virtual double GetRadius()const=0;
    virtual void SetRadius( double Radius)=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_COLLISION_COMPONENT_H