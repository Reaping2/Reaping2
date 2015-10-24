#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/collisions/flag_collision_sub_system.h"
#include "core/flag_collision_component.h"
#include "core/i_attachable_component.h"
#include "core/i_flag_carrier_component.h"
#include "core/i_team_component.h"
#include "core/map/map_system.h"
#include "core/map/ctf_flag_spawn_point_map_element.h"
#include "core/i_position_component.h"

namespace engine {
namespace ctf {

using ::ctf::FlagCollisionComponent;
using ::ctf::IAttachableComponent;
using ::ctf::IFlagCarrierComponent;

FlagCollisionSubSystem::FlagCollisionSubSystem()
    : CollisionSubSystem()
{
}


void FlagCollisionSubSystem::Init()
{
}


void FlagCollisionSubSystem::Update(Actor& actor, double DeltaTime)
{
    if (!mOther)
    {
        return;
    }
    Opt<IAttachableComponent> actorAttachableC=actor.Get<IAttachableComponent>();
    if (!actorAttachableC.IsValid())
    {
        return;
    }
    if (actorAttachableC->GetAttachedGUID()!=-1)
    {
        //TODO: this will be extended to platform handling
        return;
    }

    Opt<IFlagCarrierComponent> otherFlagCarrierC(mOther->Get<IFlagCarrierComponent>());
    if (!otherFlagCarrierC.IsValid())
    {
        return;
    }
    Opt<ITeamComponent> otherTeamC(mOther->Get<ITeamComponent>());
    if (!otherTeamC.IsValid())
    {
        return;
    }
    Opt<ITeamComponent> actorTeamC(actor.Get<ITeamComponent>());
    if (!actorTeamC.IsValid())
    {
        return;
    }
    if (otherTeamC->GetTeam()==actorTeamC->GetTeam())
    {
        //TODO: return flag to base
        map::MapElementListFilter<map::MapSystem::All> mapElementListFilter(map::MapSystem::Get()->GetMapElementList(),map::ctf::CtfFlagSpawnPointMapElement::GetType_static());
        for( map::MapElementListFilter<map::MapSystem::All>::const_iterator ctfFlagSpawnPointMapElementIt = mapElementListFilter.begin(), ctfFlagSpawnPointMapElementE = mapElementListFilter.end(); ctfFlagSpawnPointMapElementIt != ctfFlagSpawnPointMapElementE; ++ctfFlagSpawnPointMapElementIt )
        {
            Opt<map::ctf::CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement(*ctfFlagSpawnPointMapElementIt);
            if(ctfFlagSpawnPointMapElement->GetTeam()==actorTeamC->GetTeam())
            {
                Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
                if (positionC.IsValid())
                {
                    positionC->SetX(ctfFlagSpawnPointMapElement->GetX());
                    positionC->SetY(ctfFlagSpawnPointMapElement->GetY());
                    positionC->SetOrientation(0.0);
                }
                break;
            }
        }
    }
    else
    {
        //touching enemy flag
        actorAttachableC->SetAttachedGUID(mOther->GetGUID());
    }


}


void FlagCollisionSubSystem::ClipScene(Actor& actor)
{
    CollisionSubSystem::ClipScene(actor);
    Opt<FlagCollisionComponent> flagCC=actor.Get<FlagCollisionComponent>();
}

} // namespace ctf
} // namespace engine

