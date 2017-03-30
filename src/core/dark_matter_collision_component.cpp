#include "core/dark_matter_collision_component.h"

DarkMatterCollisionComponent::DarkMatterCollisionComponent()
    : CollisionComponent()
{
}



void DarkMatterCollisionComponentLoader::BindValues()
{
}

DarkMatterCollisionComponentLoader::DarkMatterCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}


REAPING2_CLASS_EXPORT_IMPLEMENT( DarkMatterCollisionComponent, DarkMatterCollisionComponent );
