#include "core/map/level_generator/room_start_property.h"
#include "spawn_property.h"
#include "../group_map_element.h"
#include "../soldier_spawn_point_map_element.h"


namespace map {

RoomStartProperty::RoomStartProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
{
}

void RoomStartProperty::Load( Json::Value& setters )
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


void RoomStartProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

RoomStartProperty::Targets_t const& RoomStartProperty::GetTargets() const
{
    return mTargets;
}



void RoomStartProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos )
{
    if (roomDesc.HasProperty( RoomProperty::Start ))
    {
        static int32_t componentId = AutoId( "position_component" );
        static Opt<MapSystem> mapSystem = MapSystem::Get();
        int currTargetIndex = 0;
        while (currTargetIndex < mTargets.size())
        {
            int32_t target = mTargets[currTargetIndex];
            for (auto targetMapElement : MapElementListFilter<MapSystem::UID>( mMapElementHolder.mAllMapElements, target ))
            {
                if (targetMapElement->GetType() == SoldierSpawnPointMapElement::GetType_static())
                {
                    Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement( targetMapElement );
                    soldierSpawnPointMapElement->SetX( soldierSpawnPointMapElement->GetX() + pos.x );
                    soldierSpawnPointMapElement->SetY( soldierSpawnPointMapElement->GetY() + pos.y );
                    MapSystem::Get()->GetMapElementList().insert( targetMapElement );
                }
                else if (targetMapElement->GetType() == GroupMapElement::GetType_static())
                {
                    Opt<GroupMapElement> groupMapElement( targetMapElement );
                    mTargets.insert( mTargets.end(), groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
                }
            }
            ++currTargetIndex;
        }
    }
}

} // namespace map
