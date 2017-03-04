#include "platform/i_platform.h"
#include "map_system.h"
#include "platform/auto_id.h"
#include "core/map_loaded_event.h"
#include "engine/engine.h"
#include "map_element_removed_event.h"
#include "platform/filesystem_utils.h"

namespace map {

MapSystem::MapSystem()
    : mScene( Scene::Get() )
    , mMapElementFactory ( MapElementFactory::Get() )
{
}


void MapSystem::Init()
{
    mOnMapLoad = EventServer<core::MapLoadEvent>::Get().Subscribe( boost::bind( &MapSystem::OnMapLoad, this, _1 ) );
}


void MapSystem::Update( double DeltaTime )
{
}

MapSystem::~MapSystem()
{
    ClearMapElements();
}

void MapSystem::ClearMapElements()
{
    for( MapElementList_t::iterator it = mMapElementHolder.mAllMapElements.begin(), e = mMapElementHolder.mAllMapElements.end(); it != e; ++it )
    {
        delete ( *it ).Get();
    }
    mMapElementHolder.mAllMapElements.clear();
}


void MapSystem::OnMapLoad( core::MapLoadEvent const& Evt )
{
    ClearMapElements();
    fs_utils::for_each( Evt.mMapName, ".json", [&]( Json::Value const& desc )
    {
        std::string name;
        if (!Json::GetStr( desc["name"], name ))
        {
            return;
        }
        std::auto_ptr<MapElement> mapElement( mMapElementFactory( AutoId( name ) ) );
        mapElement->Load( desc );
        mMapElementHolder.mAllMapElements.insert( Opt<MapElement>( mapElement.release() ) );
    } );
    EventServer<core::MapLoadedEvent>::Get().SendEvent( core::MapLoadedEvent() );

}

Opt<MapSystem> MapSystem::Get()
{
    return engine::Engine::Get().GetSystem<MapSystem>();
}

MapElementList_t& MapSystem::GetMapElementList()
{
    return mMapElementHolder.mAllMapElements;
}

void MapSystem::RemoveMapElement( int32_t spawnedActorGUID )
{
    for( MapElementList_t::iterator it = mMapElementHolder.mAllMapElements.begin(), e = mMapElementHolder.mAllMapElements.end(); it != e; ++it )
    {
        if ( ( *it )->GetSpawnedActorGUID() == spawnedActorGUID )
        {
            EventServer<MapElementRemovedEvent>::Get().SendEvent( MapElementRemovedEvent( *it ) );
            delete ( *it ).Get();
            mMapElementHolder.mAllMapElements.erase( it );
            return;
        }
    }
}

void MapSystem::RemoveMapElementUID( int32_t mapElementUID )
{
    for (MapElementList_t::iterator it = mMapElementHolder.mAllMapElements.begin(), e = mMapElementHolder.mAllMapElements.end(); it != e; ++it)
    {
        if ((*it)->GetUID() == mapElementUID)
        {
            EventServer<MapElementRemovedEvent>::Get().SendEvent( MapElementRemovedEvent( *it ) );
            delete (*it).Get();
            mMapElementHolder.mAllMapElements.erase( it );
            return;
        }
    }
}


Opt<MapElement> MapSystem::GetMapElement( int32_t spawnedActorGUID )
{
    for (auto mapElement : mMapElementHolder.mAllMapElements)
    {
        if (mapElement->GetSpawnedActorGUID() == spawnedActorGUID)
        {
            return mapElement;
        }
    }
    return Opt<MapElement>();
}

int32_t MapSystem::GetNextUniqueSpawnIndex( std::string const& startsWith )
{
    int max = 0;
    static IdStorage& idStorage = IdStorage::Get();
    for (auto mapElement : MapSystem::Get()->GetMapElementList())
    {
        try
        {
            std::string mapElementUName;
            if (idStorage.GetName( mapElement->GetIdentifier(), mapElementUName ))
            {
                if (boost::starts_with( mapElementUName, startsWith ))
                {
                    max = std::max( std::stoi( mapElementUName.substr( 1 ) ), max );
                }
            }
        }
        catch (...) {}
    }
    return ++max;
}

MapElementHolder::MapElementHolder( MapElementHolder const& holder )
{
    for (auto elem : holder.mAllMapElements)
    {
        mAllMapElements.insert(elem->clone());
    }
}


MapElementIdentifierModifier::MapElementIdentifierModifier( int32_t newIdentifier )
    : mNewIdentifier( newIdentifier )
{

}


void MapElementIdentifierModifier::operator()( Opt<MapElement>& mapElement )
{
    mapElement->SetIdentifier( mNewIdentifier );
}

} // namespace map

