#ifndef INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
#define INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H

#include "collision_component.h"

class Actor;
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

#endif//INCLUDED_CORE_PICKUP_COLLISION_COMPONENT_H
