#ifndef INCLUDED_MAP_CTF_FLAG_SPAWN_POINT_H
#define INCLUDED_MAP_CTF_FLAG_SPAWN_POINT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "core/ctf_program_state.h"

namespace map {
namespace ctf {
class CtfFlagSpawnPointMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( CtfFlagSpawnPointMapElement )
    CtfFlagSpawnPointMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    void Save( Json::Value& Element );
    void SetX( int32_t x );
    int32_t GetX()const;
    void SetY( int32_t y );
    int32_t GetY()const;
    void SetTeam( Team::Type team );
    Team::Type GetTeam()const;
private:
    int32_t mX;
    int32_t mY;
    Team::Type mTeam;
};
} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_FLAG_SPAWN_POINT_H

//command:  "classgenerator.exe" -c "ctf_flag_spawn_point" -g "map_element" -m "int32_t-x int32_t-y Team::Type-team"
