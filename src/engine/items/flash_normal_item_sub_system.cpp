#include "engine/items/flash_normal_item_sub_system.h"
#include "normal_item_sub_system.h"
#include "core/i_position_component.h"
#include "engine/items/flash_event.h"

namespace engine {

FlashNormalItemSubSystem::FlashNormalItemSubSystem()
    : mScene( Scene::Get() )
    , mNormalItemSubSystem( NormalItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}


void FlashNormalItemSubSystem::Init()
{
}


void FlashNormalItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedItem( ItemType::Normal );
    if ( normalItem->IsUse() )
    {
        Opt<IMoveComponent> actorMoveC = actor.Get<IMoveComponent>();
        Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
        if ( !actorPositionC.IsValid() )
        {
            return;
        }
        double originalX = actorPositionC->GetX();
        double originalY = actorPositionC->GetY();
        const double h = actorPositionC->GetOrientation();
        const double c = cos( h );
        const double s = sin( h );
        actorPositionC->SetX( actorPositionC->GetX() + c * 300 );
        actorPositionC->SetY( actorPositionC->GetY() + s * 300 );
        EventServer<FlashEvent>::Get().SendEvent( FlashEvent( actor.GetGUID(), originalX, originalY, actorPositionC->GetX(), actorPositionC->GetY() ) );
        //TODO: do some stuff with it
        normalItem->SetConsumed( true );
    }
}

} // namespace engine

