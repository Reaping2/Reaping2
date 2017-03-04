#ifndef INCLUDED_MAP_CTF_SPAWN_SOLDIERS_H
#define INCLUDED_MAP_CTF_SPAWN_SOLDIERS_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_input.h"

namespace map {
namespace ctf {

class CtfSpawnSoldiersMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE( CtfSpawnSoldiersMapElement )
    CtfSpawnSoldiersMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    static int32_t SpawnNodeId();
private:
};

} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SPAWN_SOLDIERS_H

//command:  "classgenerator.exe" -c "ctf_spawn_soldiers" -g "map_element"
