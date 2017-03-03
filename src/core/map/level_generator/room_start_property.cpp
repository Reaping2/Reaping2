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

void RoomStartProperty::Load( Json::Value const& setters )
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


void RoomStartProperty::Save( Json::Value& setters ) const
{
    IProperty::Save( setters );
    auto& idStorage = IdStorage::Get();
    Json::Value TargetArr( Json::arrayValue );
    for (auto& target : mTargets)
    {
        std::string targetName;
        if (idStorage.GetName( target, targetName ))
        {
            Json::Value jName = Json::Value( targetName );
            TargetArr.append( jName );
        }
    }
    setters["targets"] = TargetArr;
}

void RoomStartProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

RoomStartProperty::Targets_t const& RoomStartProperty::GetTargets() const
{
    return mTargets;
}



void RoomStartProperty::Generate( RoomDesc& roomDesc, MapElementHolder& mMapElementHolder, glm::vec2 pos, bool editor /*= false*/ )
{
    if (roomDesc.HasProperty( RoomProperty::Start ) || editor)
    {
        static Opt<MapSystem> mapSystem = MapSystem::Get();
        int currTargetIndex = 0;
        auto targets = mTargets;
        while (currTargetIndex < targets.size())
        {
            int32_t target = targets[currTargetIndex];
            for (auto targetMapElement : MapElementListFilter<MapSystem::Identifier>( mMapElementHolder.mAllMapElements, target ))
            {
                if (targetMapElement->GetType() == SoldierSpawnPointMapElement::GetType_static())
                {
                    Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement( Opt<map::MapElement>(targetMapElement->clone()) );
                    soldierSpawnPointMapElement->SetX( soldierSpawnPointMapElement->GetX() + pos.x );
                    soldierSpawnPointMapElement->SetY( soldierSpawnPointMapElement->GetY() + pos.y );
                    mapSystem->GetMapElementList().insert( soldierSpawnPointMapElement );
                }
                else if (targetMapElement->GetType() == GroupMapElement::GetType_static())
                {
                    Opt<GroupMapElement> groupMapElement( Opt<map::MapElement>( targetMapElement->clone() ) );
                    targets.insert( targets.end(), groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
                    MapSystem::Get()->GetMapElementList().insert( groupMapElement );
                }
            }
            ++currTargetIndex;
        }
    }
}

} // namespace map
