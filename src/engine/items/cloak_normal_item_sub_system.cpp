#include "engine/items/cloak_normal_item_sub_system.h"
#include "normal_item_sub_system.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"

namespace engine {

CloakNormalItemSubSystem::CloakNormalItemSubSystem()
    : mScene( Scene::Get() )
    , mNormalItemSubSystem( NormalItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
    , mProjectileId( AutoId( "CloakBuff" ) )
{
}


void CloakNormalItemSubSystem::Init()
{
}


void CloakNormalItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedItem( ItemType::Normal );
    if ( normalItem->IsUse() )
    {
        Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
        if ( buffHolderC.IsValid() )
        {
            buffHolderC->AddBuff( core::BuffFactory::Get()( mProjectileId ) );
        }

        //TODO: do some stuff with it
        normalItem->SetConsumed( true );
    }
}

} // namespace engine

