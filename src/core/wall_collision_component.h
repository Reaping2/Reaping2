#ifndef INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
#define INCLUDED_CORE_WALL_COLLISION_COMPONENT_H

#include "collision_component.h"

class WallCollisionComponent : public CollisionComponent
{
public:
    virtual void Collide( Actor& Other );
protected:
    WallCollisionComponent();
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
