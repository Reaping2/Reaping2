#include "platform/i_platform.h"
#include "spawn_actor_map_element_system.h"
#include "core/map/spawn_actor_map_element.h"
#include "../actor_factory.h"

namespace map {

SpawnActorMapElementSystem::SpawnActorMapElementSystem()
    : MapElementSystem()
    , mActorFactory(ActorFactory::Get())
{
}


void SpawnActorMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void SpawnActorMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update(DeltaTime);
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),SpawnActorMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator spawnActorMapElementIt = mapElementListFilter.begin(), spawnActorMapElementE = mapElementListFilter.end(); spawnActorMapElementIt != spawnActorMapElementE; ++spawnActorMapElementIt )
    {
        Opt<SpawnActorMapElement> spawnActorMapElement(*spawnActorMapElementIt);
        if (spawnActorMapElement->GetValueId(SpawnActorMapElement::SpawnNodeId())>0)
        {
            std::auto_ptr<Actor> actor(mActorFactory(spawnActorMapElement->GetActorID()));
            ActorCreator::ComponentLoaderMap_t const& componentLoaders=spawnActorMapElement->GetComponentLoaders();
            for(ActorCreator::ComponentLoaderMap_t::const_iterator i=componentLoaders.begin(), e=componentLoaders.end();i!=e;++i)
            {
                i->second->FillProperties(actor.get());
            }
            spawnActorMapElement->SetSpawnedActorGUID(actor->GetGUID());
            L1("spawned actor: %d",actor->GetGUID());
            mScene.AddActor(actor.release());
        }
        spawnActorMapElement->ResetValues();
    }
}


} // namespace map

