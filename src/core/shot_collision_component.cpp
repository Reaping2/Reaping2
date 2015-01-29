#include "core/shot_collision_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/component_factory.h"
#include "core/i_collision_component.h"
#include "core/scene.h"
#include "platform/i_platform.h"

ShotCollisionComponent::ShotCollisionComponent()
    : CollisionComponent()
    , mDamage( 0 )
    , mParentGuid( -1 )
{

}

void ShotCollisionComponent::SetDamage(int32_t Damage)
{
    mDamage=Damage;
}

int32_t ShotCollisionComponent::GetDamage() const
{
    return mDamage;
}

void ShotCollisionComponent::SetParent( Actor* Parent )
{
    mParentGuid = Parent->GetGUID();
}

int32_t ShotCollisionComponent::GetParentGuid() const
{
    return mParentGuid;
}

void ShotCollisionComponentLoader::BindValues()
{
    Bind("damage",func_int32_t(&ShotCollisionComponent::SetDamage));
}

ShotCollisionComponentLoader::ShotCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}
