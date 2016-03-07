#ifndef INCLUDED_MAP_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"

namespace map {

class SpawnSoldiersMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( SpawnSoldiersMapElementSystem )
    SpawnSoldiersMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    core::ProgramState& mProgramState;
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_SOLDIERS_MAP_ELEMENT_SYSTEM_H


