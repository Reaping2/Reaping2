#include "platform/i_platform.h"
#include "core/drop_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/scene.h"
#include "core/actor_factory.h"
#include "core/pickup_collision_component.h"
#include "platform/auto_id.h"
#include <boost/assert.hpp>

using platform::AutoId;

void DropOnDeathComponent::Update(double Seconds)
{
    BOOST_ASSERT(mActor && mActor->Get<IHealthComponent>().IsValid());
    if(!mTriedDrop&&!mActor->Get<IHealthComponent>()->IsAlive())
    {
        mTriedDrop=true;
#ifdef DEBUG
        static const size_t Mod = 3;
#else
        static const size_t Mod = 10;
#endif//DEBUG
        if( rand() % Mod )
        {
            return;
        }
        std::auto_ptr<Actor> Pu=ActorFactory::Get()(AutoId("pickup"));
        Pu->AddAction( AutoId( "fade_out" ) );
        int32_t contentId=AutoId(rand() % 2 ? "pistol" : "plasma_gun");
        Pu->Get<PickupCollisionComponent>()->SetPickupContent( contentId );
        Pu->SetId(contentId);
        BOOST_ASSERT(mActor->Get<IPositionComponent>().IsValid());
        Opt<IPositionComponent> positionC = mActor->Get<IPositionComponent>();
        Opt<IPositionComponent> puPositionC = Pu->Get<IPositionComponent>();
        puPositionC->SetX(positionC->GetX());
        puPositionC->SetY(positionC->GetY());
        Scene::Get().AddActor( Pu.release() );
    }
}

DropOnDeathComponent::DropOnDeathComponent()
    : mTriedDrop(false)
{

}

DropOnDeathComponentLoader::DropOnDeathComponentLoader()
{

}

void DropOnDeathComponentLoader::BindValues()
{

}
