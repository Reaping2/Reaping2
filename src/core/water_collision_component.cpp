#include "core/water_collision_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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

BOOST_CLASS_EXPORT_IMPLEMENT(WaterCollisionComponent);
