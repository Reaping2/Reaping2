#ifndef INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/map_start_event.h"
#include "soldier_spawn_point_map_element_system.h"
#include "core/ctf_program_state.h"

namespace map {
namespace ctf {

class CtfSoldierSpawnPointMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(CtfSoldierSpawnPointMapElementSystem)
    CtfSoldierSpawnPointMapElementSystem();
    SpawnPoints_t GetActiveSpawnPoints(::ctf::Team::Type team);
    static Opt<CtfSoldierSpawnPointMapElementSystem> Get();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnMapStart;
    void OnMapStart(core::MapStartEvent const& Evt);
};
} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -c "ctf_soldier_spawn_point_map_element_system" -g "map_element_system" -t "ctf_soldier_spawn_point_map_elemnt" -e "core-mapStart"
