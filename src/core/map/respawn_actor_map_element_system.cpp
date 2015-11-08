#include "platform/i_platform.h"
#include "respawn_actor_map_element_system.h"
#include "core/map/respawn_actor_map_element.h"
#include "../i_health_component.h"

namespace map {

RespawnActorMapElementSystem::RespawnActorMapElementSystem()
    : MapElementSystem()
    , mActorFactory(ActorFactory::Get())
{
}


void RespawnActorMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void RespawnActorMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update(DeltaTime);
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),RespawnActorMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator respawnActorMapElementIt = mapElementListFilter.begin(), respawnActorMapElementE = mapElementListFilter.end(); respawnActorMapElementIt != respawnActorMapElementE; ++respawnActorMapElementIt )
    {
        Opt<RespawnActorMapElement> respawnActorMapElement(*respawnActorMapElementIt);
        if (respawnActorMapElement->GetValueId(RespawnActorMapElement::SpawnNodeId)>0)
        {
            SpawnActor(respawnActorMapElement);
            respawnActorMapElement->ResetValues();
        }
        Opt<Actor> actor(mScene.GetActor(respawnActorMapElement->GetSpawnedActorGUID()));
        bool actorDead=!actor.IsValid();
        if (actor.IsValid())
        {
            Opt<IHealthComponent> healthC(actor->Get<IHealthComponent>());
            actorDead=actorDead||healthC.IsValid()&&!healthC->IsAlive();
        }
        if (actorDead)
        {
            respawnActorMapElement->SetSecsToRespawn(respawnActorMapElement->GetSecsToRespawn()-DeltaTime);
            if (respawnActorMapElement->GetSecsToRespawn()<=0)
            {
                SpawnActor(respawnActorMapElement);
            }
        }

    }
}

void RespawnActorMapElementSystem::SpawnActor(Opt<RespawnActorMapElement> respawnActorMapElement)
{
    std::auto_ptr<Actor> actor(mActorFactory(respawnActorMapElement->GetActorID()));
    ActorCreator::ComponentLoaderMap_t const& componentLoaders=respawnActorMapElement->GetComponentLoaders();
    for(ActorCreator::ComponentLoaderMap_t::const_iterator i=componentLoaders.begin(), e=componentLoaders.end();i!=e;++i)
    {
        i->second->FillProperties(actor.get());
    }
    respawnActorMapElement->SetSpawnedActorGUID(actor->GetGUID());
    respawnActorMapElement->SetSecsToRespawn(respawnActorMapElement->GetSecsToRespawnOriginal());
    mScene.AddActor(actor.release());
}


} // namespace map

