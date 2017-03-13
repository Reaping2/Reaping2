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
#include "../item_changed_event.h"

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
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<IControllerComponent>();
    if ( !playerControllerC.IsValid() )
    {
        return;
    }
    HandleInputs( actor, playerControllerC );
    Shoot( actor, playerControllerC );
    HandleReload( actor, playerControllerC );
    SetSpeedAndHeading( actor, playerControllerC );
    SetOrientation( actor, playerControllerC );
    HandleItemSwitch( actor, playerControllerC, ItemType::Normal );
    HandleItemSwitch( actor, playerControllerC, ItemType::Weapon );
}

void PlayerControllerSubSystem::SetSpeedAndHeading( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    if (mProgramState.mMode == core::ProgramState::Server)
    {
        return;
    }
    if (mProgramState.mMode == core::ProgramState::Client
        && mProgramState.mControlledActorGUID != actor.GetGUID())
    {
        return;
    }
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
    if (mProgramState.mMode == core::ProgramState::Client
        && mProgramState.mControlledActorGUID != actor.GetGUID())
    {
        return;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    BOOST_ASSERT( inventoryC.IsValid() );
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( weapon.IsValid() )
    {
        weapon->SetShoot( playerControllerC->mShoot );
        weapon->SetShootAlt( playerControllerC->mShootAlt );
    }

    Opt<NormalItem> normalItem = inventoryC->GetSelectedItem( ItemType::Normal );
    if ( normalItem.IsValid() )
    {
        normalItem->SetUse( playerControllerC->mUseNormalItem.GetValue() );
    }
    playerControllerC->mUseNormalItem.Handled();
}

void PlayerControllerSubSystem::SetOrientation( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    if (mProgramState.mMode == core::ProgramState::Server)
    {
        return;
    }
    if (mProgramState.mMode == core::ProgramState::Client 
        && mProgramState.mControlledActorGUID != actor.GetGUID())
    {
        return;
    }
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
    if (mProgramState.mMode == core::ProgramState::Server)
    {
        return;
    }
    if (mProgramState.mControlledActorGUID != actor.GetGUID())
    {
        return;
    }
    Opt<core::ClientData> clientData( mProgramState.FindClientDataByActorGUID( actor.GetGUID() ) );
    if( !clientData.IsValid() )
    {
        return;
    }
    auto const& inputState = mInputSystem->GetInputState( clientData->mControlledLocalPlayerId );
    playerControllerC->mOrientation = inputState.mOrientation;
    playerControllerC->mShoot = inputState.mShoot;
    playerControllerC->mShootAlt = inputState.mShootAlt;
    if (inputState.mUseNormalItem)
    {
        playerControllerC->mUseNormalItem.Activate();
    }
    else
    {
        playerControllerC->mUseNormalItem.Deactivate();
    }
    if (inputState.mActivate)
    {
        playerControllerC->mActivate.Activate();
    }
    else
    {
        playerControllerC->mActivate.Deactivate();
    }
    if (inputState.mSwitchWeapon)
    {
        playerControllerC->mSwitchWeapon.Activate();
    }
    else
    {
        playerControllerC->mSwitchWeapon.Deactivate();
    }
    if (inputState.mSwitchNormalItem)
    {
        playerControllerC->mSwitchNormalItem.Activate();
    }
    else
    {
        playerControllerC->mSwitchNormalItem.Deactivate();
    }
    if (inputState.mReload)
    {
        playerControllerC->mUseReload.Activate();
    }
    else
    {
        playerControllerC->mUseReload.Deactivate();
    }
    playerControllerC->mMoving = inputState.mMoving;
    playerControllerC->mHeading = inputState.mHeading;
}

void PlayerControllerSubSystem::HandleReload( Actor& actor, Opt<PlayerControllerComponent> playerControllerC )
{
    if (mProgramState.mMode == core::ProgramState::Client
        && mProgramState.mControlledActorGUID != actor.GetGUID())
    {
        return;
    }
    if ( !playerControllerC->mUseReload.GetValue() )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    BOOST_ASSERT( inventoryC.IsValid() );
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( weapon.IsValid() )
    {
        if ( weapon->CanReload() )
        {
            weapon->Reload();
            EventServer<ItemPropertiesChangedEvent>::Get().SendEvent( ItemPropertiesChangedEvent( *weapon ) );
        }
    }
    if ( mProgramState.mMode != core::ProgramState::Client )
    {
        playerControllerC->mUseReload.Handled();
    }
}


void PlayerControllerSubSystem::HandleItemSwitch( Actor& actor, Opt<PlayerControllerComponent> playerControllerC, ItemType::Type itemType )
{
    if (mProgramState.mMode == core::ProgramState::Client)
    {
        return;
    }
    if (itemType == ItemType::Weapon && !playerControllerC->mSwitchWeapon.GetValue()
        || itemType == ItemType::Normal && !playerControllerC->mSwitchNormalItem.GetValue())
    {
        return;
    }
    auto inventoryC(actor.Get<IInventoryComponent>());
    BOOST_ASSERT( inventoryC.IsValid() );
    auto prevItem( inventoryC->GetSelectedItem( itemType ) );
    auto currItem( inventoryC->SwitchToNextItem( itemType ) );
    if (currItem.IsValid() && prevItem.IsValid()
        && currItem->GetId() != prevItem->GetId() )
    {
        EventServer<ItemChangedEvent>::Get().SendEvent( ItemChangedEvent( actor.GetGUID(), itemType, currItem->GetId(), prevItem->GetId() ) );
    }
    if (itemType == ItemType::Weapon)
    {
        playerControllerC->mSwitchWeapon.Handled();
    }
    else if (itemType == ItemType::Normal)
    {
        playerControllerC->mSwitchNormalItem.Handled();
    }
}

} // namespace engine

