#ifndef INCLUDED_CORE_FLAG_COLLISION_COMPONENT_H
#define INCLUDED_CORE_FLAG_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"

namespace ctf {

class FlagCollisionComponent : public CollisionComponent
{
public:
    FlagCollisionComponent();
protected:
    friend class ComponentFactory;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FlagCollisionComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<CollisionComponent>(*this);
}

class FlagCollisionComponentLoader : public ComponentLoader<FlagCollisionComponent>
{
    virtual void BindValues();
public:
    FlagCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


} // namespace ctf

#endif//INCLUDED_CORE_FLAG_COLLISION_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "flag_collision_component" -p "collision_component"
