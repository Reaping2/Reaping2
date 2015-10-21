#ifndef INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_H
#define INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "../ctf_program_state.h"

namespace map {
namespace ctf {
class CtfSoldierSpawnPointMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE(CtfSoldierSpawnPointMapElement)
    CtfSoldierSpawnPointMapElement();
    void Load(Json::Value& setters);
    void SetX(int32_t x);
    int32_t GetX()const;
    void SetY(int32_t y);
    int32_t GetY()const;
    void SetTeam(::ctf::Team::Type team);
    ::ctf::Team::Type GetTeam()const;
private:
    int32_t mX;
    int32_t mY;
    ::ctf::Team::Type mTeam;
};
} // namespace ctf
} // namespace map

#endif//INCLUDED_MAP_CTF_SOLDIER_SPAWN_POINT_H

//command:  "classgenerator.exe" -c "ctf_soldier_spawn_point" -g "map_element" -m "int32_t-x int32_t-y ctf::Team-team"
