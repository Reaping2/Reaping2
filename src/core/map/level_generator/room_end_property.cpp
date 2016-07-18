#include "core/map/level_generator/room_end_property.h"
#include "spawn_property.h"


namespace map {

RoomEndProperty::RoomEndProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
{
}

void RoomEndProperty::Load( Json::Value& setters )
{
    IProperty::Load( setters );
    mTargets.clear();
    auto const& targets = setters["targets"];
    if (targets.isArray())
    {
        for (auto& target : targets)
        {
            mTargets.push_back( AutoId( target.asString() ) );
        }
    }
}


void RoomEndProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

RoomEndProperty::Targets_t const& RoomEndProperty::GetTargets() const
{
    return mTargets;
}



void RoomEndProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos )
{
    if (roomDesc.HasProperty( RoomProperty::End ))
    {
        SpawnProperty::SpawnTargets( roomDesc, mTargets, mMapElementHolder, pos );
    }
}

} // namespace map
