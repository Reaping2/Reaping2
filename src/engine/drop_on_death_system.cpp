#include "platform/i_platform.h"
#include "engine/drop_on_death_system.h"
#include "core/i_collision_component.h"
#include "boost/assert.hpp"
#include "core/collision_model.h"
#include "core/i_drop_on_death_component.h"
#include "core/i_health_component.h"
#include <stdlib.h>
#include "core/pickup_collision_component.h"
#include "core/i_position_component.h"

namespace engine {

DropOnDeathSystem::DropOnDeathSystem()
    : mScene( Scene::Get() )
    , mActorFactory(ActorFactory::Get())
{

}

void DropOnDeathSystem::Init()
{
}

void DropOnDeathSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IDropOnDeathComponent> dropOnDeathC=actor.Get<IDropOnDeathComponent>();
        if (!dropOnDeathC.IsValid())
        {
            continue;
        }

        if(!dropOnDeathC->IsTriedDrop()&&!actor.Get<IHealthComponent>()->IsAlive())
        {
            dropOnDeathC->SetTriedDrop(true);
#ifdef DEBUG
            static const size_t Mod = 3;
#else
            static const size_t Mod = 10;
#endif//DEBUG
            if( rand() % Mod )
            {
                return;
            }
            std::auto_ptr<Actor> Pu=mActorFactory(AutoId("pickup"));
            Pu->AddAction( AutoId( "fade_out" ) );
            int32_t contentId=AutoId(rand() % 2 ? "pistol" : "plasma_gun");
            Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
            Pu->SetId(contentId);
            BOOST_ASSERT(actor.Get<IPositionComponent>().IsValid());
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            Opt<IPositionComponent> puPositionC = Pu->Get<IPositionComponent>();
            puPositionC->SetX(positionC->GetX());
            puPositionC->SetY(positionC->GetY());
            Scene::Get().AddActor( Pu.release() );
        }
    }

}

} // namespace engine

