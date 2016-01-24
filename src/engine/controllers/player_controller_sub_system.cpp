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

namespace engine {

PlayerControllerSubSystem::PlayerControllerSubSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}

void PlayerControllerSubSystem::Init()
{
    mInputSystem=InputSystem::Get();
}

void PlayerControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<PlayerControllerComponent>();
    if (!playerControllerC.IsValid())
    {
        return;
    }
    if (mProgramState.mMode==core::ProgramState::Client)
    {
        playerControllerC->mReloadTyped=false;
    }

    if(playerControllerC->mActive)
    {
        HandleInputs(actor,playerControllerC);
    }

    HandleReload(actor,playerControllerC);
    Shoot(actor,playerControllerC);
    if (mProgramState.mMode==core::ProgramState::Client)
    {
        return;
    }

    SetSpeedAndOrientation(actor,playerControllerC);
    SetOrientation(actor,playerControllerC);
}

void PlayerControllerSubSystem::SetSpeedAndOrientation(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        return;
    }
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    moveC->SetMoving(playerControllerC->mMoving);
    actor.Get<IMoveComponent>()->SetHeading( playerControllerC->mHeading );
}

void PlayerControllerSubSystem::Shoot(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC=actor.Get<IInventoryComponent>();
    BOOST_ASSERT(inventoryC.IsValid());
    Opt<Weapon> weapon=inventoryC->GetSelectedWeapon();
    if (weapon.IsValid())
    {
        weapon->SetShoot(playerControllerC->mShoot);
        weapon->SetShootAlt(playerControllerC->mShootAlt);
    }

    Opt<NormalItem> normalItem = inventoryC->GetSelectedNormalItem();
    if (normalItem.IsValid())
    {
        normalItem->SetUse(playerControllerC->mUseNormalItem);
    }
}

void PlayerControllerSubSystem::SetOrientation(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        return;
    }
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    actorPositionC->SetOrientation( playerControllerC->mOrientation );
}

void PlayerControllerSubSystem::HandleInputs(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    playerControllerC->mOrientation=mInputSystem->GetInputState().mOrientation;
    playerControllerC->mShoot=mInputSystem->GetInputState().mShoot;
    playerControllerC->mShootAlt=mInputSystem->GetInputState().mShootAlt;
    playerControllerC->mUseNormalItem=mInputSystem->GetInputState().mUseNormalItem;
    playerControllerC->mReloadTyped=mInputSystem->GetInputState().mReload;
    playerControllerC->mMoving=mInputSystem->GetInputState().mMoving;
    playerControllerC->mHeading=mInputSystem->GetInputState().mHeading;
}

void PlayerControllerSubSystem::HandleReload(Actor& actor, Opt<PlayerControllerComponent> playerControllerC)
{
    if (!playerControllerC->mReloadTyped)
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC=actor.Get<IInventoryComponent>();
    BOOST_ASSERT(inventoryC.IsValid());
    Opt<Weapon> weapon=inventoryC->GetSelectedWeapon();
    if (weapon.IsValid())
    {
        if (weapon->CanReload())
        {
            weapon->SetBullets(0.0);
        }
    }
    if (mProgramState.mMode!=core::ProgramState::Client)
    {
        playerControllerC->mReloadTyped=false;
    }
}

} // namespace engine

