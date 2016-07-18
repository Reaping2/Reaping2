#include "platform/i_platform.h"
#include "group_map_element_system.h"
#include "core/map/group_map_element.h"

namespace map {

GroupMapElementSystem::GroupMapElementSystem()
    : MapElementSystem()
{
}


void GroupMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void GroupMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update( DeltaTime );
    for( Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), GroupMapElement::GetType_static() ) )
    {
    }
}


} // namespace map

