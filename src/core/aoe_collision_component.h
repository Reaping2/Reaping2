#ifndef INCLUDED_CORE_AOE_COLLISION_COMPONENT_H
#define INCLUDED_CORE_AOE_COLLISION_COMPONENT_H

#include "collision_component.h"
#include "core/property_loader.h"
#include <set>
#include <boost/serialization/set.hpp>
#include "platform/export.h"
class AoeCollisionComponent : public CollisionComponent
{
public:
    typedef std::set<int32_t> Damaged_Actor_Ids_t;
    AoeCollisionComponent();
    virtual void AddDamagedActorId( int32_t damagedActorId );
    virtual Damaged_Actor_Ids_t const& GetDamagedActorIds()const;
    virtual void SetDamageOnce( bool damageOnce );
    virtual bool IsDamageOnce()const;
    virtual void SetDamage( int32_t damage );
    virtual int32_t GetDamage()const;
protected:
    friend class ComponentFactory;
    Damaged_Actor_Ids_t mDamagedActorIds;
    bool mDamageOnce;
    int32_t mDamage;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void AoeCollisionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<CollisionComponent>( *this );
    ar& mDamagedActorIds;
    ar& mDamageOnce;
    ar& mDamage;
}

class AoeCollisionComponentLoader : public ComponentLoader<AoeCollisionComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( AoeCollisionComponentLoader )
private:
    virtual void BindValues();
protected:
    AoeCollisionComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( AoeCollisionComponent, AoeCollisionComponent, "aoe_collision_component" );
#endif//INCLUDED_CORE_AOE_COLLISION_COMPONENT_H

