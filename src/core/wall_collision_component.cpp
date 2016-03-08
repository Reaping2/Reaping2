#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/wall_collision_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>


WallCollisionComponent::WallCollisionComponent()
    : CollisionComponent()
{
}

void WallCollisionComponentLoader::BindValues()
{

}

WallCollisionComponentLoader::WallCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( WallCollisionComponent, WallCollisionComponent );
