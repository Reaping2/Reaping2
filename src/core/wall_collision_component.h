#ifndef INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
#define INCLUDED_CORE_WALL_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "core/property_loader.h"

class WallCollisionComponent : public CollisionComponent
{
protected:
    WallCollisionComponent();
    friend class ComponentFactory;
};

class WallCollisionComponentLoader: public ComponentLoader<WallCollisionComponent>
{
    virtual void BindValues();
protected:
    WallCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
