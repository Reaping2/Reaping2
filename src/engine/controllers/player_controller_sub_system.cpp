#include "platform/i_platform.h"
#include "engine/controllers/player_controller_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "input/keyboard.h"
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
    , mSpaceTyped(false)
{
    mKeyId = EventServer<KeyEvent>::Get().Subscribe( boost::bind( &PlayerControllerSubSystem::OnKeyEvent, this, _1 ) );
}

void PlayerControllerSubSystem::Init()
{
    mKeyboard=Engine::Get().GetSystem<KeyboardSystem>();
    mMouse=Engine::Get().GetSystem<MouseSystem>();
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &PlayerControllerSubSystem::OnMouseMoveEvent, this, _1 ) );
}

void PlayerControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<PlayerControllerComponent>();
    if (!playerControllerC.IsValid())
    {
        return;
    }

    if(playerControllerC->mActive)
    {
        HandleInputs(actor,playerControllerC);
    }

    if (mProgramState.mMode==core::ProgramState::Client)
    {
        return;
    }

    SetSpeedAndOrientation(actor,playerControllerC);
    Shoot(actor,playerControllerC);
    SetOrientation(actor,playerControllerC);
    HandleRevive(actor,playerControllerC);
}

void PlayerControllerSubSystem::OnMouseMoveEvent(const WorldMouseMoveEvent& Event)
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

void PlayerControllerSubSystem::SetSpeedAndOrientation(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        return;
    }
    int x = ( ( playerControllerC->mCurrentMovement & MF_Left ) ? -1 : 0 ) + ( ( playerControllerC->mCurrentMovement & MF_Right ) ? 1 : 0 );
    int y = ( ( playerControllerC->mCurrentMovement & MF_Up ) ? 1 : 0 ) + ( ( playerControllerC->mCurrentMovement & MF_Down ) ? -1 : 0 );

    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    moveC->SetMoving(std::max<double>( std::abs( x ), std::abs( y ) )!=0);

    double Heading = 0;
    static const double pi = boost::math::constants::pi<double>();
    if( x == 0 )
    {
        Heading = ( y < 0 ) ? -pi / 2 : pi / 2;
    }
    else if( y == 0 )
    {
        Heading = ( x < 0 ) ? -pi : 0;
    }
    else if( y > 0 )
    {
        Heading = ( x < 0 ) ? pi * 0.75 : pi / 4;
    }
    else
    {
        Heading = ( x < 0 ) ? pi * 1.25 : pi * 1.75;
    }
//    double hed=std::floor(Heading*PRECISION)/PRECISION;
//     L1("heading: %f,prec: %f cos: %f sin: %f, cosreal: %f sinreal: %f\n",Heading,hed
//         ,glm::round(cos( hed )*PRECISION/10)/PRECISION*10,glm::round(sin( hed )*PRECISION/10)/PRECISION*10
//         ,glm::round(cos( hed )*PRECISION)/PRECISION,glm::round(sin( hed )*PRECISION)/PRECISION);
    actor.Get<IMoveComponent>()->SetHeading( Heading );
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
    playerControllerC->mCurrentMovement = 0;
    if( mKeyboard->GetKey(GLFW_KEY_W).State==KeyState::Down)
    {
        playerControllerC->mCurrentMovement |= MF_Up;
    }
    if( mKeyboard->GetKey(GLFW_KEY_A).State==KeyState::Down )
    {
        playerControllerC->mCurrentMovement |= MF_Left;
    }
    if( mKeyboard->GetKey(GLFW_KEY_S).State==KeyState::Down )
    {
        playerControllerC->mCurrentMovement |= MF_Down;
    }
    if( mKeyboard->GetKey(GLFW_KEY_D).State==KeyState::Down )
    {
        playerControllerC->mCurrentMovement |= MF_Right;
    }

    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    playerControllerC->mOrientation = atan2( mY - actorPositionC->GetY(), mX - actorPositionC->GetX() );

    if (mMouse->IsButtonPressed( MouseSystem::Button_Left ))
    {
        playerControllerC->mShoot=true;
        playerControllerC->mShootAlt=false;
    }
    else if (mMouse->IsButtonPressed( MouseSystem::Button_Right ))
    {
        playerControllerC->mShoot=false;
        playerControllerC->mShootAlt=true;
    }
    else
    {
        playerControllerC->mShoot=false;
        playerControllerC->mShootAlt=false;
    }

    if( mKeyboard->GetKey(GLFW_KEY_Q).State==KeyState::Down )
    {
        playerControllerC->mUseNormalItem=true;
    }
    else
    {
        playerControllerC->mUseNormalItem=false;
    }
    
    if( mSpaceTyped )
    {
        playerControllerC->mReviveTyped=true;
        L2("space Typed for revive!\n");
    }
    mSpaceTyped=false;
}

void PlayerControllerSubSystem::OnKeyEvent(const KeyEvent& Event)
{
    if( Event.Key == GLFW_KEY_SPACE && Event.State == KeyState::Up )
    {
        mSpaceTyped=true;
        L2("space typed!\n");
    }
}

void PlayerControllerSubSystem::HandleRevive(Actor &actor, Opt<PlayerControllerComponent> playerControllerC)
{
    if (!playerControllerC->mReviveTyped)
    {
        return;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid()&&!healthC->IsAlive())
    {
        L2("revive!\n");
        Opt<core::ClientData> clientData(mProgramState.FindClientDataByActorGUID(actor.GetGUID()));
        if(clientData.IsValid())
        {
            std::auto_ptr<Actor> player(engine::SoldierSpawnSystem::Get()->Spawn(*clientData));
            if (player.get())
            {
                player->Get<PlayerControllerComponent>()->SetEnabled(true);
                player->Get<PlayerControllerComponent>()->mActive=true;
                playerControllerC->SetEnabled(false);
                playerControllerC->mActive=false;
                mProgramState.mControlledActorGUID=clientData->mClientActorGUID;
                mScene.SetPlayerModels(Opt<Actor>(player.get()));
                mScene.AddActor(player.release());
            }
        }
    }
    else
    {
        L1("%s health is not available, or actor still alive:%d\n",__FUNCTION__,actor.GetGUID());
    }
    playerControllerC->mReviveTyped=false;
}

} // namespace engine

