#ifndef INCLUDED_MAP_EDITOR_SOLDIER_SPAWN_SYSTEM_H
#define INCLUDED_MAP_EDITOR_SOLDIER_SPAWN_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "../map_start_event.h"
#include "../actor_factory.h"
#include "ctf_soldier_spawn_point_map_element.h"

namespace map {

class EditorSoldierSpawnSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( EditorSoldierSpawnSystem )
    EditorSoldierSpawnSystem();
    static void Spawn( Opt<ctf::CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ActorFactory& mActorFactory;
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_SOLDIER_SPAWN_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_soldier_spawn_system" -e "core-mapStart"
