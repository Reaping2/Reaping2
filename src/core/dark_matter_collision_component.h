#ifndef INCLUDED_CORE_DARK_MATTER_COLLISION_COMPONENT_H
#define INCLUDED_CORE_DARK_MATTER_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class DarkMatterCollisionComponent : public CollisionComponent
{
public:
    DarkMatterCollisionComponent();
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void DarkMatterCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<CollisionComponent>(*this);
}

class DarkMatterCollisionComponentLoader : public ComponentLoader<DarkMatterCollisionComponent>
{
    virtual void BindValues();
protected:
    DarkMatterCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( DarkMatterCollisionComponent, DarkMatterCollisionComponent, "dark_matter_collision_component" );

#endif//INCLUDED_CORE_DARK_MATTER_COLLISION_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "dark_matter_collision_component" -p "collision_component"
