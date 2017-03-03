#ifndef INCLUDED_MAP_RESPAWN_ACTOR_H
#define INCLUDED_MAP_RESPAWN_ACTOR_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "core/actor_factory.h"
#include "base_input.h"

namespace map {

class RespawnActorMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE( RespawnActorMapElement )
    RespawnActorMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    void Save( Json::Value& Element );
    void SetActorID( int32_t actorID );
    int32_t GetActorID()const;
    ActorCreator::ComponentLoaderMap_t const& GetComponentLoaders()const;
    void AddComponentLoader( int32_t componentId, std::auto_ptr<PropertyLoaderBase<Component> > compLoader );
    Opt<PropertyLoaderBase<Component>> GetComponentLoader( int32_t componentId );
    void SetSecsToRespawn( double secsToRespawn );
    double GetSecsToRespawn()const;
    void SetSecsToRespawnOriginal( double secsToRespawnOriginal );
    double GetSecsToRespawnOriginal()const;
    static int32_t SpawnNodeId();
    RespawnActorMapElement& operator=( RespawnActorMapElement const& other );
    RespawnActorMapElement( RespawnActorMapElement const& other );
private:
    int32_t mActorID;
    ActorCreator::ComponentLoaderMap_t mComponentLoaders;
    double mSecsToRespawn;
    double mSecsToRespawnOriginal;
    // operator= should be filled
};

} // namespace map

#endif//INCLUDED_MAP_RESPAWN_ACTOR_H

//command:  "classgenerator.exe" -g "map_element" -c "respawn_actor" -m "int32_t-actorID ActorCreator::ComponentLoaderMap_t-componentLoaders double-secsToRespawn int32_t-spawnedActorGUID double-secsToRespawnOriginal"
