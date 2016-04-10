#include "platform/i_platform.h"
#include "engine/controllers/player_controller_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/player_controller_component.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/i_inventory_component.h"
#include "core/weapon.h"
#include "core/i_health_component.h"
#include "../soldier_spawn_system.h"
#include "../item_properties_changed_event.h"

namespace engine {

PlayerControllerSubSystem::PlayerControllerSubSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}

void PlayerControllerSubSystem::Init()
{
    mInputSystem = InputSystem::Get();
}

void PlayerControllerSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<PlayerControllerComponent>();
    if ( !playerControllerC.IsValid() )
    {
        return;
    }
    if ( mProgramState.mMode == core::ProgramState::Server )
    {
        Shoot( actor, playerControllerC );
        HandleReload( actor, playerControllerC );
    }
    else if( playerControllerC->mActive )
    {
        HandleInputs( actor, playerControllerC );
        Shoot( actor, playerControllerC );
        HandleReload( actor, playerControllerC );
        SetSpeedAndOrientation( actor, playerControllerC );
        SetOrientation( actor, playerControllerC );
    }
}

void PlayerControllerSubSystem::SetSpeedAndOrientation( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return;
    }
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    moveC->SetMoving( playerControllerC->mMoving );
    actor.Get<IMoveComponent>()->SetHeading( playerControllerC->mHeading );
}

void PlayerControllerSubSystem::Shoot( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    BOOST_ASSERT( inventoryC.IsValid() );
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    if ( weapon.IsValid() )
    {
        weapon->SetShoot( playerControllerC->mShoot );
        weapon->SetShootAlt( playerControllerC->mShootAlt );
    }

    Opt<NormalItem> normalItem = inventoryC->GetSelectedNormalItem();
    if ( normalItem.IsValid() )
    {
        normalItem->SetUse( playerControllerC->mUseNormalItem.GetValue() );
    }
    playerControllerC->mUseNormalItem.Handled();
}

void PlayerControllerSubSystem::SetOrientation( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return;
    }
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    actorPositionC->SetOrientation( playerControllerC->mOrientation );
}

void PlayerControllerSubSystem::HandleInputs( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    playerControllerC->mOrientation = mInputSystem->GetInputState().mOrientation;
    playerControllerC->mShoot = mInputSystem->GetInputState().mShoot;
    playerControllerC->mShootAlt = mInputSystem->GetInputState().mShootAlt;
    if (mInputSystem->GetInputState().mUseNormalItem)
    {
        playerControllerC->mUseNormalItem.Activate();
    }
    else
    {
        playerControllerC->mUseNormalItem.Deactivate();
    }
    if (mInputSystem->GetInputState().mReload)
    {
        playerControllerC->mUseReload.Activate();
    }
    else
    {
        playerControllerC->mUseReload.Deactivate();
    }
    playerControllerC->mMoving = mInputSystem->GetInputState().mMoving;
    playerControllerC->mHeading = mInputSystem->GetInputState().mHeading;
}

void PlayerControllerSubSystem::HandleReload( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    if ( !playerControllerC->mUseReload.GetValue() )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    BOOST_ASSERT( inventoryC.IsValid() );
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    if ( weapon.IsValid() )
    {
        if ( weapon->CanReload() )
        {
            weapon->SetBullets( 0.0 );
            EventServer<ItemPropertiesChangedEvent>::Get().SendEvent( ItemPropertiesChangedEvent( *weapon ) );
        }
    }
    if ( mProgramState.mMode != core::ProgramState::Client )
    {
        playerControllerC->mUseReload.Handled();
    }
}

} // namespace engine

