#ifndef INCLUDED_MAP_SPAWN_ACTOR_H
#define INCLUDED_MAP_SPAWN_ACTOR_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "core/actor_factory.h"
#include "core/component_loader_factory.h"
#include "base_input.h"

namespace map {

class SpawnActorMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE(SpawnActorMapElement)
    SpawnActorMapElement(int32_t Id);
    void Load(Json::Value& setters);
    void Save(Json::Value& Element);
    static void LoadComponentLoaders(Json::Value& setters, ActorCreator::ComponentLoaderMap_t& mComponentLoaders);
    void SetSpawnedActorGUID(int32_t spawnedActorGUID);
    int32_t GetSpawnedActorGUID()const;
    void SetActorID(int32_t actorID);
    int32_t GetActorID()const;
    ActorCreator::ComponentLoaderMap_t const& GetComponentLoaders()const;
    void AddComponentLoader(int32_t componentId, std::auto_ptr<ActorCreator::ComponentLoader_t> compLoader);
    static const int32_t SpawnNodeId;
private:
    int32_t mActorID;
    int32_t mSpawnedActorGUID;
    ActorCreator::ComponentLoaderMap_t mComponentLoaders;
    ComponentLoaderFactory& mComponentLoaderFactory;
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_ACTOR_H

//command:  "classgenerator.exe" -g "map_element" -c "spawn_actor" -m "int32_t-actorID ActorCreator::ComponentLoaderMap_t-componentLoaders"
