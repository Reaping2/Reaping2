#include "core/map/ctf_soldier_spawn_point_map_element.h"
#include "../actor.h"
#include "../scene.h"

namespace map {
namespace ctf {

CtfSoldierSpawnPointMapElement::CtfSoldierSpawnPointMapElement( int32_t Id )
    : MapElement( Id )
    , mX( 0.0 )
    , mY( 0.0 )
    , mTeam( Team::Blue )
{
}

void CtfSoldierSpawnPointMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    Json::Value const& position = setters["position"];
    if( !position.isObject() )
    {
        return;
    }
    int32_t x;
    if (Json::GetInt( position["x"], x ))
    {
        SetX( x );
    }
    int32_t y;
    if (Json::GetInt( position["y"], y ))
    {
        SetY( y );
    }
    std::string teamStr;
    if ( Json::GetStr( setters["team"], teamStr ) )
    {
        SetTeam( Team::Get()(AutoId( teamStr )) );
    }

}

void CtfSoldierSpawnPointMapElement::SetX( int32_t x )
{
    mX = x;
}

int32_t CtfSoldierSpawnPointMapElement::GetX()const
{
    return mX;
}

void CtfSoldierSpawnPointMapElement::SetY( int32_t y )
{
    mY = y;
}

int32_t CtfSoldierSpawnPointMapElement::GetY()const
{
    return mY;
}

void CtfSoldierSpawnPointMapElement::SetTeam( Team::Type team )
{
    mTeam = team;
}

Team::Type CtfSoldierSpawnPointMapElement::GetTeam()const
{
    return mTeam;
}

void CtfSoldierSpawnPointMapElement::Save( Json::Value& Element )
{
    Opt<Actor> actor( Scene::Get().GetActor( mSpawnedActorGUID ) );
    if ( !actor.IsValid() )
    {
        return;
    }
    MapElement::Save( Element );

    Element["team"] = Json::Value( mTeam == Team::Blue ? "blue" : "red" );
    Json::Value positionObj( Json::objectValue );

    positionObj["x"] = mX;
    positionObj["y"] = mY;

    Element["position"] = positionObj;
}

} // namespace ctf
} // namespace map
