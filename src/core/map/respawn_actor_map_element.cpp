#include "core/map/respawn_actor_map_element.h"
#include "spawn_actor_map_element.h"

namespace map {

const int RespawnActorMapElement::SpawnNodeId=AutoId("spawn");

RespawnActorMapElement::RespawnActorMapElement(int32_t Id)
    : MapElement(Id)
    , BaseInput()
    , mActorID(-1)
    , mSecsToRespawn(100)
    , mSecsToRespawnOriginal(100)
    , mSpawnedActorGUID(-1)
{
}

void RespawnActorMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    AddInputNodeId(SpawnNodeId);

    std::string actorStr;
    if (!Json::GetStr(setters["actor"],actorStr))
    {
        return;
    }
    SetActorID(AutoId(actorStr));

    SpawnActorMapElement::LoadComponentLoaders(setters,mComponentLoaders);

    Json::GetDouble(setters["secs_to_respawn"],mSecsToRespawn);
    mSecsToRespawnOriginal=mSecsToRespawn;
}

void RespawnActorMapElement::SetActorID(int32_t actorID)
{
    mActorID=actorID;
}

int32_t RespawnActorMapElement::GetActorID()const
{
    return mActorID;
}

ActorCreator::ComponentLoaderMap_t const& RespawnActorMapElement::GetComponentLoaders()const
{
    return mComponentLoaders;
}

void RespawnActorMapElement::SetSecsToRespawn(double secsToRespawn)
{
    mSecsToRespawn=secsToRespawn;
}

double RespawnActorMapElement::GetSecsToRespawn()const
{
    return mSecsToRespawn;
}

void RespawnActorMapElement::SetSecsToRespawnOriginal(double secsToRespawnOriginal)
{
    mSecsToRespawnOriginal=secsToRespawnOriginal;
}

double RespawnActorMapElement::GetSecsToRespawnOriginal()const
{
    return mSecsToRespawnOriginal;
}

void RespawnActorMapElement::SetSpawnedActorGUID(int32_t spawnedActorGUID)
{
    mSpawnedActorGUID=spawnedActorGUID;
}

int32_t RespawnActorMapElement::GetSpawnedActorGUID()const
{
    return mSpawnedActorGUID;
}


} // namespace map
