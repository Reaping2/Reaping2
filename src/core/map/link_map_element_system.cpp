#include "platform/i_platform.h"
#include "link_map_element_system.h"
#include "map_system.h"
#include "link_map_element.h"

namespace map {

LinkMapElementSystem::LinkMapElementSystem()
    : mScene( Scene::Get() )
{
}


void LinkMapElementSystem::Init()
{
    mMapSystem=MapSystem::Get();
    mOnMapLoaded=EventServer<core::MapLoadedEvent>::Get().Subscribe( boost::bind( &LinkMapElementSystem::OnMapLoaded, this, _1 ) );
}


void LinkMapElementSystem::Update(double DeltaTime)
{

}

void LinkMapElementSystem::OnMapLoaded(core::MapLoadedEvent const& Evt)
{
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),LinkMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator linkMapElementIt = mapElementListFilter.begin(), linkMapElementE = mapElementListFilter.end(); linkMapElementIt != linkMapElementE; ++linkMapElementIt )
    {
        Opt<LinkMapElement> linkMapElement(*linkMapElementIt);
    }
}


} // namespace map

