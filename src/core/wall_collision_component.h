#ifndef INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
#define INCLUDED_CORE_WALL_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class WallCollisionComponent : public CollisionComponent
{
protected:
    WallCollisionComponent();
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void WallCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<CollisionComponent>(*this);
}

class WallCollisionComponentLoader: public ComponentLoader<WallCollisionComponent>
{
    virtual void BindValues();
protected:
    WallCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(WallCollisionComponent,"wall_collision_component");
#endif//INCLUDED_CORE_WALL_COLLISION_COMPONENT_H
