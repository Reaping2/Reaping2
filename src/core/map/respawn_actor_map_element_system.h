#ifndef INCLUDED_MAP_RESPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_RESPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/actor_factory.h"
#include "respawn_actor_map_element.h"

namespace map {

class RespawnActorMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(RespawnActorMapElementSystem)
    RespawnActorMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    void SpawnActor(Opt<RespawnActorMapElement> respawnActorMapElement);

private:
    ActorFactory& mActorFactory;
};

} // namespace map

#endif//INCLUDED_MAP_RESPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "respawn_actor_map_element_system" -t "respawn_actor_map_element"
