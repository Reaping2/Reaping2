#include "platform/i_platform.h"
#include "spawn_actor_map_element_system.h"
#include "core/map/spawn_actor_map_element.h"
#include "../actor_factory.h"
#include "engine/engine.h"

namespace map {

SpawnActorMapElementSystem::SpawnActorMapElementSystem()
    : MapElementSystem()
    , mActorFactory( ActorFactory::Get() )
    , mRemoveWhenUsed( true )
{
}

void SpawnActorMapElementSystem::SetRemoveMapElementWhenUsed( bool removeWhenUsed )
{
    mRemoveWhenUsed = removeWhenUsed;
}

bool SpawnActorMapElementSystem::IsRemoveMapElementWhenUsed()
{
    return mRemoveWhenUsed;
}

void SpawnActorMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void SpawnActorMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    std::vector<int32_t> mapElementUIDs;
    for (Opt<SpawnActorMapElement> spawnActorMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), SpawnActorMapElement::GetType_static() ))
    {
        if ( spawnActorMapElement->GetValueId( SpawnActorMapElement::SpawnNodeId() ) > 0 )
        {
            std::auto_ptr<Actor> actor( mActorFactory( spawnActorMapElement->GetActorID() ) );
            ActorCreator::ComponentLoaderMap_t const& componentLoaders = spawnActorMapElement->GetComponentLoaders();
            for( ActorCreator::ComponentLoaderMap_t::const_iterator i = componentLoaders.begin(), e = componentLoaders.end(); i != e; ++i )
            {
                i->second->FillProperties( *actor.get() );
            }
            spawnActorMapElement->SetSpawnedActorGUID( actor->GetGUID() );
            L2( "spawned actor: %d", actor->GetGUID() );
            mScene.AddActor( actor.release() );
            if (mRemoveWhenUsed&&spawnActorMapElement->IsRemoveWhenUsed())
            {
                mapElementUIDs.push_back( spawnActorMapElement->GetUID() );
            }
        }
        spawnActorMapElement->ResetValues();
    }
    for (auto uid : mapElementUIDs)
    {
        mMapSystem->RemoveMapElementUID( uid );
    }
}


RemoveWhenUsedRAII::RemoveWhenUsedRAII( bool removeWhenUsed )
    : mRemoveWhenUsed( removeWhenUsed )
    , mOriginal( false )
{
    auto sames(engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>());
    mOriginal = sames->IsRemoveMapElementWhenUsed();
    sames->SetRemoveMapElementWhenUsed( mRemoveWhenUsed );
}

RemoveWhenUsedRAII::~RemoveWhenUsedRAII()
{
    auto sames( engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>() );
    sames->SetRemoveMapElementWhenUsed( mOriginal );
}

} // namespace map

