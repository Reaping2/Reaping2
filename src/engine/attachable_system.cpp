#include "platform/i_platform.h"
#include "attachable_system.h"
#include "core/i_attachable_component.h"
#include "core/i_position_component.h"
#include "core/i_health_component.h"

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


void AttachableSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<IAttachableComponent> attachableC=actor.Get<IAttachableComponent>();
       if (!attachableC.IsValid())
       {
           continue;
       }
       if (attachableC->GetAttachedGUID()==-1)
       {
           continue;
       }

       Opt<Actor> attachedActor(mScene.GetActor(attachableC->GetAttachedGUID()));
       if (!attachedActor.IsValid())
       {
           attachableC->SetAttachedGUID(-1);
           continue;
       }

       Opt<IHealthComponent> attachedHealthC(attachedActor->Get<IHealthComponent>());
       if (attachedHealthC.IsValid())
       {
           if (!attachedHealthC->IsAlive())
           {
               attachableC->SetAttachedGUID(-1);
               continue;
           }
       }

       Opt<IPositionComponent> attachedPositionC(attachedActor->Get<IPositionComponent>());
       if (!attachedPositionC.IsValid())
       {
           continue;
       }
       Opt<IPositionComponent> positionC(actor.Get<IPositionComponent>());
       if (!positionC.IsValid())
       {
           continue;
       }

       positionC->SetX(attachedPositionC->GetX());
       positionC->SetY(attachedPositionC->GetY());
       positionC->SetOrientation(attachedPositionC->GetOrientation()+boost::math::double_constants::pi);
    }
}

} // namespace ctf
} // namespace engine

