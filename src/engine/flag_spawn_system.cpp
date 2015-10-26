#include "platform/i_platform.h"
#include "flag_spawn_system.h"
#include "core/map/map_system.h"
#include "core/map/ctf_flag_spawn_point_map_element.h"
#include "core/i_position_component.h"
#include "core/i_team_component.h"
#include "engine.h"

namespace engine {
namespace ctf {

FlagSpawnSystem::FlagSpawnSystem()
    : mScene( Scene::Get() )
{
}


void FlagSpawnSystem::Init()
{
}


void FlagSpawnSystem::Update(double DeltaTime)
{
}

Opt<FlagSpawnSystem> FlagSpawnSystem::Get()
{
    return Engine::Get().GetSystem<FlagSpawnSystem>();
}

void FlagSpawnSystem::SetFlagPositionToStart(Actor& actor)
{
    Opt<ITeamComponent> actorTeamC(actor.Get<ITeamComponent>());
    if (!actorTeamC.IsValid())
    {
        return;
    }
    Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
    if (!positionC.IsValid())
    {
        return;
    }

    map::MapElementListFilter<map::MapSystem::All> mapElementListFilter(map::MapSystem::Get()->GetMapElementList(),map::ctf::CtfFlagSpawnPointMapElement::GetType_static());
    for( map::MapElementListFilter<map::MapSystem::All>::const_iterator ctfFlagSpawnPointMapElementIt = mapElementListFilter.begin(), ctfFlagSpawnPointMapElementE = mapElementListFilter.end(); ctfFlagSpawnPointMapElementIt != ctfFlagSpawnPointMapElementE; ++ctfFlagSpawnPointMapElementIt )
    {
        Opt<map::ctf::CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement(*ctfFlagSpawnPointMapElementIt);
        if(ctfFlagSpawnPointMapElement->GetTeam()==actorTeamC->GetTeam())
        {
            positionC->SetX(ctfFlagSpawnPointMapElement->GetX());
            positionC->SetY(ctfFlagSpawnPointMapElement->GetY());
            positionC->SetOrientation(0.0);
            break;
        }
    }
}

} // namespace ctf
} // namespace engine

