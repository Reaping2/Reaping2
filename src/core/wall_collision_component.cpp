#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/wall_collision_component.h"
#include "core/actor.h"
#include "platform/i_platform.h"


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
