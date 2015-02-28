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
            static const size_t Mod = 2;
#else
            static const size_t Mod = 2;
#endif//DEBUG
            if( rand() % Mod )
            {
                return;
            }
            std::auto_ptr<Actor> Pu=mActorFactory(AutoId("pickup"));
            if(rand() % 2)
            {
                int32_t contentId=Roll(3);
                Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
                Pu->Get<PickupCollisionComponent>()->SetItemType( Item::Weapon );
            }
            else
            {
                Pu->Get<PickupCollisionComponent>()->SetPickupContent( AutoId("grenade_normal_item") );
                Pu->Get<PickupCollisionComponent>()->SetItemType( Item::Normal );
            }
            BOOST_ASSERT(actor.Get<IPositionComponent>().IsValid());
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            Opt<IPositionComponent> puPositionC = Pu->Get<IPositionComponent>();
            puPositionC->SetX(positionC->GetX());
            puPositionC->SetY(positionC->GetY());
            Scene::Get().AddActor( Pu.release() );
        }
    }

}

AutoId DropOnDeathSystem::Roll( int32_t n )
{
	// TODO: need a way to ask about available drops, this way
	switch(rand() % n)
	{
	case 0:
		return AutoId("pistol");
	case 1:
		return AutoId("plasma_gun");
	case 2:
		return AutoId("rocket_launcher");
	}
	BOOST_ASSERT(false);
	return AutoId("default_item");
}

} // namespace engine

