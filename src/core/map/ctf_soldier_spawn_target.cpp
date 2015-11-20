#include "ctf_soldier_spawn_target.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "ctf_soldier_spawn_point_map_element.h"
#include "editor_soldier_spawn_system.h"
#include "../i_team_component.h"
#include "../i_move_component.h"
namespace map {

CtfSoldierSpawnTarget::CtfSoldierSpawnTarget(int32_t Id, int32_t curosrId, Team::Type team)
    : ITarget(Id)
    , mCursorId(curosrId)
    , mActorId(Id)
    , mTeam(team)
    , mScene(Scene::Get())
{

}

void CtfSoldierSpawnTarget::Update(double DeltaTime)
{

}

int32_t CtfSoldierSpawnTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t CtfSoldierSpawnTarget::GetActorId() const
{
    return mActorId;
}

void CtfSoldierSpawnTarget::PutTarget(glm::vec2 position)
{
    std::auto_ptr<MapElement> mapElement(MapElementFactory::Get()(AutoId("ctf_soldier_spawn_point")));
    Opt<ctf::CtfSoldierSpawnPointMapElement> ctfSoldierSpawn(static_cast<ctf::CtfSoldierSpawnPointMapElement*>(mapElement.get()));
    ctfSoldierSpawn->SetTeam(mTeam);
    ctfSoldierSpawn->SetX(position.x);
    ctfSoldierSpawn->SetY(position.y);
    EditorSoldierSpawnSystem::Spawn(ctfSoldierSpawn);
    MapSystem::Get()->GetMapElementList().insert(Opt<MapElement>(mapElement.release()));
}

std::auto_ptr<Actor> CtfSoldierSpawnTarget::GetCursor()
{
    std::auto_ptr<Actor> player(ActorFactory::Get()(mCursorId));
    Opt<ITeamComponent> teamC(player->Get<ITeamComponent>());
    if (teamC.IsValid())
    {
        teamC->SetTeam(mTeam);
    }
    Opt<IMoveComponent> moveC(player->Get<IMoveComponent>());
    moveC->SetMoving(false);
    return player;
}

} // namespace map
