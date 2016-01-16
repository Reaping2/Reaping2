#include "core/water_collision_component.h"

WaterCollisionComponent::WaterCollisionComponent()
    : CollisionComponent()
{
}



void WaterCollisionComponentLoader::BindValues()
{
}

WaterCollisionComponentLoader::WaterCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}
