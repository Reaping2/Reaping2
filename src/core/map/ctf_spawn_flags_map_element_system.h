#ifndef INCLUDED_MAP_CTF_SPAWN_FLAGS_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_CTF_SPAWN_FLAGS_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/actor_factory.h"

namespace map {
namespace ctf {

class CtfSpawnFlagsMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(CtfSpawnFlagsMapElementSystem)
    CtfSpawnFlagsMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    ::core::ProgramState& mProgramState;
    ActorFactory& mActorFactory;
    int32_t mFlagAutoId;
};

} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SPAWN_FLAGS_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -c "ctf_spawn_flags_map_element_system" -g "map_element_system" -t "ctf_spawn_flags_map_element"
