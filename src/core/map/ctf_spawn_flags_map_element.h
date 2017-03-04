#ifndef INCLUDED_MAP_CTF_SPAWN_FLAGS_H
#define INCLUDED_MAP_CTF_SPAWN_FLAGS_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_input.h"

namespace map {
namespace ctf {

class CtfSpawnFlagsMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE( CtfSpawnFlagsMapElement )
    CtfSpawnFlagsMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    static int32_t SpawnNodeId();
private:
};

} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SPAWN_FLAGS_H

//command:  "classgenerator.exe" -c "ctf_spawn_flags" -g "map_element"
