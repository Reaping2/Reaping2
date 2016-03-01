#ifndef INCLUDED_CORE_WATER_COLLISION_COMPONENT_H
#define INCLUDED_CORE_WATER_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class WaterCollisionComponent : public CollisionComponent
{
public:
    WaterCollisionComponent();
protected:
    friend class ComponentFactory;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void WaterCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<CollisionComponent>(*this);
}

class WaterCollisionComponentLoader : public ComponentLoader<WaterCollisionComponent>
{
    virtual void BindValues();
protected:
    WaterCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2(WaterCollisionComponent, WaterCollisionComponent,"water_collision_component");
#endif//INCLUDED_CORE_WATER_COLLISION_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "water_collision_component" -p "collision_component"
