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
    DEFINE_MAP_ELEMENT_BASE( SpawnActorMapElement )
    SpawnActorMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    void Save( Json::Value& Element );
    static void LoadComponentLoaders( Json::Value const& setters, ActorCreator::ComponentLoaderMap_t& mComponentLoaders );
    void SetActorID( int32_t actorID );
    int32_t GetActorID()const;
    ActorCreator::ComponentLoaderMap_t const& GetComponentLoaders()const;
    void AddComponentLoader( int32_t componentId, std::auto_ptr<PropertyLoaderBase<Component> > compLoader );
    Opt<PropertyLoaderBase<Component>> GetComponentLoader( int32_t componentId );
    static int32_t SpawnNodeId();
    void SetRemoveWhenUsed( bool removeWhenUsed );
    bool IsRemoveWhenUsed() const;
    SpawnActorMapElement& operator=( SpawnActorMapElement const& other );
    SpawnActorMapElement( SpawnActorMapElement const& other );
private:
    int32_t mActorID;
    ActorCreator::ComponentLoaderMap_t mComponentLoaders;
    bool mRemoveWhenUsed;
    // operator= should be filled
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_ACTOR_H

//command:  "classgenerator.exe" -g "map_element" -c "spawn_actor" -m "int32_t-actorID ActorCreator::ComponentLoaderMap_t-componentLoaders"
