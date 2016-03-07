#ifndef INCLUDED_MAP_CTF_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_CTF_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/program_state.h"
#include "core/ctf_program_state.h"

namespace map {
namespace ctf {

class CtfSpawnSoldiersMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( CtfSpawnSoldiersMapElementSystem )
    CtfSpawnSoldiersMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    ::core::ProgramState& mProgramState;
    ::ctf::ProgramState& mCtfProgramState;
};

} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -c "ctf_spawn_soldiers_map_element_system" -g "map_element_system" -t "ctf_spawn_soldiers_map_element"
