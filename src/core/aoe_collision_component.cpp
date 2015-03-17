#include "core/aoe_collision_component.h"

AoeCollisionComponent::AoeCollisionComponent()
    : mDamagedActorIds()
    , mDamageOnce(true)
    , mDamage(10)
{
}

AoeCollisionComponent::Damaged_Actor_Ids_t const&  AoeCollisionComponent::GetDamagedActorIds()const
{
    return mDamagedActorIds;
}

void AoeCollisionComponent::SetDamageOnce(bool damageOnce)
{
    mDamageOnce=damageOnce;
}

bool AoeCollisionComponent::IsDamageOnce()const
{
    return mDamageOnce;
}

void AoeCollisionComponent::SetDamage(int32_t damage)
{
    mDamage=damage;
}

int32_t AoeCollisionComponent::GetDamage()const
{
    return mDamage;
}

void AoeCollisionComponent::AddDamagedActorId(int32_t damagedActorId)
{
    mDamagedActorIds.insert(damagedActorId);
}



void AoeCollisionComponentLoader::BindValues()
{
    Bind("damage",func_int32_t(&AoeCollisionComponent::SetDamage));
    Bind("damage_once",func_bool(&AoeCollisionComponent::SetDamageOnce));
}

AoeCollisionComponentLoader::AoeCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}
