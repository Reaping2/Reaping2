#ifndef INCLUDED_CORE_WATER_COLLISION_COMPONENT_H
#define INCLUDED_CORE_WATER_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"

class WaterCollisionComponent : public CollisionComponent
{
public:
    WaterCollisionComponent();
protected:
    friend class ComponentFactory;
private:
};

class WaterCollisionComponentLoader : public ComponentLoader<WaterCollisionComponent>
{
    virtual void BindValues();
protected:
    WaterCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_WATER_COLLISION_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "water_collision_component" -p "collision_component"
