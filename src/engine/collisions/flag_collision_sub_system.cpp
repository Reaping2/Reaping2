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
#include "core/i_flag_receiver_component.h"

namespace engine {
namespace ctf {

using ::ctf::FlagCollisionComponent;
using ::ctf::IAttachableComponent;
using ::ctf::IFlagCarrierComponent;
using ::ctf::IFlagReceiverComponent;

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

    Opt<IFlagCarrierComponent> otherFlagCarrierC(mOther->Get<IFlagCarrierComponent>());

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

    if (actorAttachableC->GetAttachedGUID()!=-1)
    {
        Opt<IFlagReceiverComponent> otherFlagReceiverC(mOther->Get<IFlagReceiverComponent>());

        if (otherFlagReceiverC.IsValid())
        {
            if (otherTeamC->GetTeam()!=actorTeamC->GetTeam())
            {
                map::MapElementListFilter<map::MapSystem::All> mapElementListFilter(map::MapSystem::Get()->GetMapElementList(),map::ctf::CtfFlagSpawnPointMapElement::GetType_static());
                for( map::MapElementListFilter<map::MapSystem::All>::const_iterator ctfFlagSpawnPointMapElementIt = mapElementListFilter.begin(), ctfFlagSpawnPointMapElementE = mapElementListFilter.end(); ctfFlagSpawnPointMapElementIt != ctfFlagSpawnPointMapElementE; ++ctfFlagSpawnPointMapElementIt )
                {
                    Opt<map::ctf::CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement(*ctfFlagSpawnPointMapElementIt);
                    if(ctfFlagSpawnPointMapElement->GetTeam()==actorTeamC->GetTeam())
                    {
                        actorAttachableC->SetAttachedGUID(-1);
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
        }
        return;
    }

    if (otherFlagCarrierC.IsValid())
    {
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

}


void FlagCollisionSubSystem::ClipScene(Actor& actor)
{
    CollisionSubSystem::ClipScene(actor);
    Opt<FlagCollisionComponent> flagCC=actor.Get<FlagCollisionComponent>();
}

} // namespace ctf
} // namespace engine

