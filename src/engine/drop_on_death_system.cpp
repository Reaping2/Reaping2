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
            int32_t rolled=rand() % 3;
            if(rolled==0)
            {
                int32_t contentId=Roll(5);
                Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
                Pu->Get<PickupCollisionComponent>()->SetItemType( ItemType::Weapon );
            }
            else if (rolled==1)
            {
                int32_t contentId=RollNormalItem(4);
                Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
                Pu->Get<PickupCollisionComponent>()->SetItemType( ItemType::Normal );
            }
            else if (rolled==2)
            {
                int32_t contentId=RollBuff(5);
                Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
                Pu->Get<PickupCollisionComponent>()->SetItemType( ItemType::Buff );
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
AutoId DropOnDeathSystem::RollNormalItem( int32_t n )
{
    // TODO: need a way to ask about available drops, this way
    switch(rand() % n)
    {
    case 0:
        return AutoId("grenade_normal_item");
    case 1:
        return AutoId("flash_normal_item");
    case 2:
        return AutoId("cloak_normal_item");
    case 3:
        return AutoId("blue_grenade_normal_item");
    }
    BOOST_ASSERT(false);
    return AutoId("default_item");
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
    case 3:
		return AutoId("shotgun");
    case 4:
		return AutoId("ion_gun");
    case 5:
		return AutoId("gatling_gun");
    }
	BOOST_ASSERT(false);
	return AutoId("default_item");
}

AutoId DropOnDeathSystem::RollBuff( int32_t n )
{
    // TODO: need a way to ask about available drops, this way
    switch(rand() % n)
    {
    case 0:
        return AutoId("HealOverTimeBuff");
    case 1:
        return AutoId("MoveSpeedBuff");
    case 2:
        return AutoId("AccuracyBuff");
    case 3:
        return AutoId("ArmorBuff");
    case 4:
        return AutoId("CloakBuff");
    }
    BOOST_ASSERT(false);
    return AutoId("DefaultBuff");
}

} // namespace engine

