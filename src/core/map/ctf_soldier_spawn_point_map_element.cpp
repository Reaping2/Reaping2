#include "core/map/ctf_soldier_spawn_point_map_element.h"

namespace map {
namespace ctf {

CtfSoldierSpawnPointMapElement::CtfSoldierSpawnPointMapElement()
    : MapElement()
    , mX(0.0)
    , mY(0.0)
    , mTeam(::ctf::Team::Blue)
{
}

void CtfSoldierSpawnPointMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    Json::Value const& position=setters["position"];
    if(!position.isObject())
    {
        return;
    }
    int32_t x;
    if (!Json::GetInt(position["x"],x))
    {
        return;
    }
    SetX(x);
    int32_t y;
    if (!Json::GetInt(position["y"],y))
    {
        return;
    }
    SetY(y);
    std::string teamStr;
    if (!Json::GetStr(setters["team"],teamStr))
    {
        return;
    }
    SetTeam(::ctf::Team::Get()(AutoId(teamStr)));

}

void CtfSoldierSpawnPointMapElement::SetX(int32_t x)
{
    mX=x;
}

int32_t CtfSoldierSpawnPointMapElement::GetX()const
{
    return mX;
}

void CtfSoldierSpawnPointMapElement::SetY(int32_t y)
{
    mY=y;
}

int32_t CtfSoldierSpawnPointMapElement::GetY()const
{
    return mY;
}

void CtfSoldierSpawnPointMapElement::SetTeam(::ctf::Team::Type team)
{
    mTeam=team;
}

::ctf::Team::Type CtfSoldierSpawnPointMapElement::GetTeam()const
{
    return mTeam;
}

} // namespace ctf
} // namespace map
