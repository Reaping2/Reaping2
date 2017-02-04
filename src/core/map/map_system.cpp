#include "platform/i_platform.h"
#include "map_system.h"
#include "platform/auto_id.h"
#include "core/map_loaded_event.h"
#include "engine/engine.h"
#include "map_element_removed_event.h"

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

bool MapSystem::AddMapElementFromOneTextureDesc( Json::Value& mapElementDesc )
{
    std::string name;
    if ( !Json::GetStr( mapElementDesc["name"], name ) )
    {
        return false;
    }
    std::auto_ptr<MapElement> mapElement( mMapElementFactory( AutoId( name ) ) );
    mapElement->Load( mapElementDesc );
    mMapElementHolder.mAllMapElements.insert( Opt<MapElement>( mapElement.release() ) );
    return true;
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
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, Evt.mMapName );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".json" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Root.isArray() )
        {
            continue;
        }
        for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
        {
            Json::Value& mapElementDesc = *i;
            if( !AddMapElementFromOneTextureDesc( mapElementDesc ) )
            {
                return;
            }
        }
    }
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

