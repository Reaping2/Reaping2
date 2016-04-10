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
#include "../attach_state_changed_event.h"

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
    mOnAttachStateChanged = EventServer<engine::AttachStateChangedEvent>::Get().Subscribe( boost::bind( &FlagCollisionSubSystem::OnAttachStateChanged, this, _1 ) );
}


void FlagCollisionSubSystem::Update( Actor& actor, double DeltaTime )
{
}

void FlagCollisionSubSystem::OnAttachStateChanged( engine::AttachStateChangedEvent const& Evt )
{
    if ( Evt.mType == AttachStateChangedEvent::Detached )
    {
        Opt<Actor> actor( mScene.GetActor( Evt.mActorGUID ) );
        if ( !actor.IsValid() )
        {
            return;
        }
        if ( !actor->Get<FlagCollisionComponent>().IsValid() )
        {
            return; //not the flag
        }
        Opt<ITeamComponent> actorTeamC;
        actorTeamC = actor->Get<ITeamComponent>();
        EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(
            ::ctf::FlagStateChangedEvent( ::ctf::FlagStateChangedEvent::Dropped, actorTeamC.IsValid() ? actorTeamC->GetTeam() : Team::None, Evt.mAttachedGUID, Evt.mActorGUID ) );
    }
}


void FlagCollisionSubSystem::ClipScene( Actor& actor )
{
    CollisionSubSystem::ClipScene( actor );
    Opt<FlagCollisionComponent> flagCC = actor.Get<FlagCollisionComponent>();
}

void FlagCollisionSubSystem::Collide( Actor& actor, Actor& other )
{
    Opt<IAttachableComponent> actorAttachableC = actor.Get<IAttachableComponent>();
    if ( !actorAttachableC.IsValid() )
    {
        return;
    }

    Opt<IFlagCarrierComponent> otherFlagCarrierC( other.Get<IFlagCarrierComponent>() );

    Opt<ITeamComponent> otherTeamC( other.Get<ITeamComponent>() );
    if ( !otherTeamC.IsValid() )
    {
        return;
    }
    Opt<ITeamComponent> actorTeamC( actor.Get<ITeamComponent>() );
    if ( !actorTeamC.IsValid() )
    {
        return;
    }

    if ( actorAttachableC->GetAttachedGUID() != -1 )
    {
        Opt<IFlagReceiverComponent> otherFlagReceiverC( other.Get<IFlagReceiverComponent>() );

        if ( otherFlagReceiverC.IsValid() )
        {
            if ( otherTeamC->GetTeam() != actorTeamC->GetTeam() )
            {
                int32_t carrierGUID = actorAttachableC->GetAttachedGUID();
                actorAttachableC->SetAttachedGUID( -1 );
                //EventServer< AttachStateChangedEvent>::Get().SendEvent(AttachStateChangedEvent(AttachStateChangedEvent::Detached,-1,actor.GetGUID()));
                FlagSpawnSystem::Get()->SetFlagPositionToStart( actor );
                EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(
                    ::ctf::FlagStateChangedEvent( ::ctf::FlagStateChangedEvent::Scored, actorTeamC->GetTeam(), carrierGUID, actor.GetGUID() ) );
                return;
            }
        }
    }

    if ( otherFlagCarrierC.IsValid() )
    {
        if ( otherTeamC->GetTeam() == actorTeamC->GetTeam() )
        {
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            if ( !positionC.IsValid() )
            {
                return;
            }
            double x = positionC->GetX();
            double y = positionC->GetY();
            FlagSpawnSystem::Get()->SetFlagPositionToStart( actor );
            if ( positionC->GetX() != x || positionC->GetY() != y )
            {
                EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(
                    ::ctf::FlagStateChangedEvent( ::ctf::FlagStateChangedEvent::Returned, actorTeamC->GetTeam(), other.GetGUID(), actor.GetGUID() ) );
            }
        }
        else
        {
            actorAttachableC->SetAttachedGUID( other.GetGUID() );
            //EventServer< AttachStateChangedEvent>::Get().SendEvent(AttachStateChangedEvent(AttachStateChangedEvent::Attached,other.GetGUID(),actor.GetGUID()));
            EventServer< ::ctf::FlagStateChangedEvent>::Get().SendEvent(
                ::ctf::FlagStateChangedEvent( ::ctf::FlagStateChangedEvent::Captured, actorTeamC->GetTeam(), other.GetGUID(), actor.GetGUID() ) );
        }
    }
}

} // namespace ctf
} // namespace engine

