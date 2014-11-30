#ifndef INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
#define INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H

#include "core/collision_component.h"

class PickupCollisionComponent : public CollisionComponent
{
public:
    virtual void Collide( Actor& Other );
    virtual void SetPickupContent( int32_t PickupContent );
protected:
    PickupCollisionComponent();
    friend class ComponentFactory;
    int32_t mPickupContent;
};

class PickupCollisionComponentLoader: public ComponentLoader<PickupCollisionComponent>
{
    virtual void BindValues();
protected:
    PickupCollisionComponentLoader();
    friend class ComponentLoaderFactory;
    CollisionClass& mCollisionClass;
};

#endif//INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
