#include "platform/i_platform.h"
#include "chest_system.h"
#include "core/i_chest_component.h"
#include "core/i_activatable_component.h"
#include "core/i_switch_component.h"
#include "platform/event.h"
#include "core/i_inventory_component.h"
#include "core/i_collision_component.h"
#include "core/pickup_collision_component.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "platform/random.h"

namespace engine {

ChestSystem::ChestSystem()
{
}


void ChestSystem::Init()
{

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IChestComponent>().IsValid(); } );
    mOnChestChanged = EventServer<ChestChangedEvent>::Get().Subscribe( std::bind( &ChestSystem::OnChestChanged, this, std::placeholders::_1 ) );

}


void ChestSystem::Update(double DeltaTime)
{
    if (mProgramState.mMode != core::ProgramState::Client)
    {
        for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
        {
            Opt<IChestComponent> chestC = actor->Get<IChestComponent>();
            if (!chestC.IsValid())
            {
                continue;
            }
            auto activatableC( actor->Get<IActivatableComponent>() );
            auto switchC( actor->Get<ISwitchComponent>() );
            if (switchC.IsValid())
            {
                if (activatableC.IsValid() && activatableC->IsActivated() && switchC->GetState() == SwitchState::Off)
                {
                    auto player( mScene.GetActor( activatableC->GetActivatorGUID() ) );
                    if (player.IsValid())
                    {
                        auto inventoryC( player->Get<IInventoryComponent>() );
                        if (inventoryC->CanPay( chestC->GetPrice() ))
                        {
                            EventServer<ChestChangedEvent>::Get().SendEvent( { actor->GetGUID(), activatableC->GetActivatorGUID(), ChestChangedEvent::StartOpen } );
                        }
                    }
                }
                if (switchC->IsStateChanged() && switchC->GetState() == SwitchState::On)
                {
                    EventServer<ChestChangedEvent>::Get().SendEvent( { actor->GetGUID(), -1, ChestChangedEvent::Opened } );
                }
            }
        }
    }
}


void ChestSystem::OnChestChanged( ChestChangedEvent const& Evt )
{
    auto actor( mScene.GetActor( Evt.mActorGUID ) );
    if (!actor.IsValid())
    {
        return;
    }
    auto switchC( actor->Get<ISwitchComponent>() );
    if (!switchC.IsValid())
    {
        return;
    }
    auto chestC = actor->Get<IChestComponent>();
    if (!chestC.IsValid())
    {
        return;
    }
    auto activatableC( actor->Get<IActivatableComponent>() );
    if (!activatableC.IsValid())
    {
        return;
    }
    if (Evt.mState == ChestChangedEvent::StartOpen)
    {
        activatableC->SetEnabled( false );
        auto player( mScene.GetActor( Evt.mPlayerGUID ) );
        if (player.IsValid())
        {
            auto chestC = actor->Get<IChestComponent>();
            if (chestC.IsValid())
            {
                auto inventoryC( player->Get<IInventoryComponent>() );
                inventoryC->Pay( chestC->GetPrice() );
            }
        }
        switchC->SetState( SwitchState::TransitionToOn );
        switchC->SetSecsToEnd( switchC->GetSecsToEndMax() );
    }
    else if (Evt.mState == ChestChangedEvent::Opened)
    {
        switchC->SetState( SwitchState::On );
        if (mProgramState.mMode != core::ProgramState::Client)
        {
            static auto& mActorFactory( ActorFactory::Get() );
            std::auto_ptr<Actor> pickup = mActorFactory( AutoId( "shop_pickup" ) );
            Opt<PickupCollisionComponent> pickupCC = pickup->Get<ICollisionComponent>();
            Opt<IPositionComponent> puPositionC = pickup->Get<IPositionComponent>();
            Opt<IMoveComponent> puMoveC = pickup->Get<IMoveComponent>();
            auto positionC = actor->Get<IPositionComponent>();
            BOOST_ASSERT( pickupCC.IsValid() && puPositionC.IsValid() && positionC.IsValid() && puMoveC.IsValid() );

            puPositionC->SetX( positionC->GetX() );
            puPositionC->SetY( positionC->GetY() );
            pickupCC->SetPickupDesc( chestC->GetPickupDesc() );
            puMoveC->SetHeading( 
                (platform::RandomGenerator::global()()*101)/100.0
                    *boost::math::constants::pi<double>() );
            puMoveC->SetSpeed( 200.0 );
            puMoveC->SetMoving( true );
            mScene.AddActor( pickup.release() );
        }
    }
}

} // namespace engine

