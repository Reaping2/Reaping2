#include "core/flag_collision_component.h"

namespace ctf {

FlagCollisionComponent::FlagCollisionComponent()
{
}



void FlagCollisionComponentLoader::BindValues()
{
}

FlagCollisionComponentLoader::FlagCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}

} // namespace ctf