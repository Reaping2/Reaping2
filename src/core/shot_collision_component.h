#ifndef INCLUDED_CORE_SHOT_COLLISION_COMPONENT_H
#define INCLUDED_CORE_SHOT_COLLISION_COMPONENT_H

#include "core/collision_component.h"
#include "core/property_loader.h"

class ShotCollisionComponent : public CollisionComponent
{
public:
    virtual void Collide( Actor& Other );
    virtual void ClipScene(); //TODO: i don't really like this concept. This might be simply just a collision with scene edges, later maybe.
    virtual void SetDamage( int32_t Damage );
    virtual int32_t GetDamage() const;
    virtual void SetParent( Actor* Parent );
protected:
    ShotCollisionComponent();
    friend class ComponentFactory;
    int32_t mDamage;
    int32_t mParentGuid;
};

class ShotCollisionComponentLoader: public ComponentLoader<ShotCollisionComponent>
{
    virtual void BindValues();
protected:
    ShotCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_SHOT_COLLISION_COMPONENT_H
