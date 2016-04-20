#include "platform/i_platform.h"
#include "attachable_system.h"
#include "core/i_attachable_component.h"
#include "core/i_position_component.h"
#include "core/i_health_component.h"
#include "core/ctf_program_state.h"
#include "core/i_team_component.h"
#include "attach_state_changed_event.h"

namespace engine {
namespace ctf {

using ::ctf::IAttachableComponent;

AttachableSystem::AttachableSystem()
    : mScene( Scene::Get() )
{
}


void AttachableSystem::Init()
{
}


void AttachableSystem::Update( double DeltaTime )
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IAttachableComponent> attachableC = actor.Get<IAttachableComponent>();
        if ( !attachableC.IsValid() )
        {
            continue;
        }
        Opt<Actor> attachedActor( mScene.GetActor( attachableC->GetAttachedGUID() ) );
        if ( attachableC->GetAttachedGUID() == -1 )
        {
            continue;
        }

        if ( !attachedActor.IsValid() )
        {
            attachableC->SetAttachedGUID( -1 );
            EventServer< AttachStateChangedEvent>::Get().SendEvent( AttachStateChangedEvent( AttachStateChangedEvent::Detached, -1, actor.GetGUID() ) );
            continue;
        }

        Opt<IHealthComponent> attachedHealthC( attachedActor->Get<IHealthComponent>() );
        if ( attachedHealthC.IsValid() )
        {
            if ( !attachedHealthC->IsAlive() )
            {
                if (attachableC->IsRemoveOnAttachedDeath())
                {
                    Opt<IHealthComponent> healthC( actor.Get<IHealthComponent>() );
                    if (healthC.IsValid())
                    {
                        healthC->SetHP( 0 );
                    }
                }
                attachableC->SetAttachedGUID( -1 );
                EventServer<AttachStateChangedEvent>::Get().SendEvent( AttachStateChangedEvent( AttachStateChangedEvent::Detached, attachedActor->GetGUID(), actor.GetGUID() ) );
                continue;
            }
        }

        Opt<IPositionComponent> attachedPositionC( attachedActor->Get<IPositionComponent>() );
        if ( !attachedPositionC.IsValid() )
        {
            continue;
        }
        Opt<IPositionComponent> positionC( actor.Get<IPositionComponent>() );
        if ( !positionC.IsValid() )
        {
            continue;
        }
        glm::vec2 rvec = glm::rotate(glm::vec2(attachableC->GetPositionX(), attachableC->GetPositionY()), float( attachedPositionC->GetOrientation() ) );
        positionC->SetX( attachedPositionC->GetX() + rvec.x );
        positionC->SetY( attachedPositionC->GetY() + rvec.y );
        if (attachableC->IsInheritOrientation())
        {
            positionC->SetOrientation( attachedPositionC->GetOrientation() + boost::math::double_constants::pi );
        }
    }
}

} // namespace ctf
} // namespace engine

