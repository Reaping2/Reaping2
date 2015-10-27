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
#include "../flag_spawn_system.h"
#include "core/flag_state_changed_event.h"

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
                actorAttachableC->SetAttachedGUID(-1);
                FlagSpawnSystem::Get()->SetFlagPositionToStart(actor);
                EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(::ctf::FlagStateChangedEvent(::ctf::FlagStateChangedEvent::Scored,actorTeamC->GetTeam()));
            }
        }
        return;
    }

    if (otherFlagCarrierC.IsValid())
    {
        if (otherTeamC->GetTeam()==actorTeamC->GetTeam())
        {
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            if (!positionC.IsValid())
            {
                return;
            }
            double x=positionC->GetX();
            double y=positionC->GetY();
            FlagSpawnSystem::Get()->SetFlagPositionToStart(actor);
            if (positionC->GetX()!=x||positionC->GetY()!=y)
            {
                EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(::ctf::FlagStateChangedEvent(::ctf::FlagStateChangedEvent::Returned,actorTeamC->GetTeam()));
            }
        }
        else
        {
            EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(::ctf::FlagStateChangedEvent(::ctf::FlagStateChangedEvent::Captured,actorTeamC->GetTeam()));
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

