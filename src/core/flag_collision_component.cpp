#include "core/flag_collision_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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
BOOST_CLASS_EXPORT_IMPLEMENT(ctf::FlagCollisionComponent);
