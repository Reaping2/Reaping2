#ifndef INCLUDED_CORE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_COLLISION_COMPONENT_H
#include "core/i_collision_component.h"
#include "core/property_loader.h"

class CollisionComponent : public ICollisionComponent
{
public:
    virtual CollisionClass::Type GetCollisionClass()const;
    virtual void SetCollisionClass( CollisionClass::Type CollisionClassType );
    virtual void Collide( Actor& Other );
    virtual void ClipScene(); //TODO: i don't really like this concept. This might be simply just a collision with scene edges, later maybe.
    virtual double GetRadius()const;
    virtual void SetRadius( double Radius);

protected:
    CollisionComponent();
    friend class ComponentFactory;
    CollisionClass::Type mCollisionClassType;
    double mRadius;
};

class CollisionComponentLoader: public ComponentLoader<CollisionComponent>
{
    virtual void BindValues();
protected:
    CollisionComponentLoader();
    friend class ComponentLoaderFactory;
    CollisionClass& mCollisionClass;
};

#endif//INCLUDED_CORE_COLLISION_COMPONENT_H
