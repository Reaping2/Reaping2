#ifndef INCLUDED_MAP_SPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_SPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/actor_factory.h"

namespace map {

class SpawnActorMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( SpawnActorMapElementSystem )
    SpawnActorMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    ActorFactory& mActorFactory;
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_ACTOR_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "spawn_actor_map_element_system" -t "spawn_actor_map_element"
