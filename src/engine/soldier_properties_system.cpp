#include "platform/i_platform.h"
#include "soldier_properties_system.h"
#include "ui/ui.h"
#include "soldier_properties_ready_event.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/max_health_buff.h"
#include "buffs_engine/max_health_buff_sub_system.h"
#include "core/i_accuracy_component.h"
#include "core/buffs/accuracy_buff.h"
#include "buffs_engine/accuracy_buff_sub_system.h"
#include "core/i_health_component.h"

namespace engine {

SoldierPropertiesSystem::SoldierPropertiesSystem()
    : mScene( Scene::Get() )
    , mProgramState ( core::ProgramState::Get() )
    , mSoldierModel( "soldier", &RootModel::Get() )
    , mHealthPressModel( StringFunc ( this, &SoldierPropertiesSystem::HealthPress ), "health_press", &mSoldierModel )
    , mAccuracyPressModel( StringFunc ( this, &SoldierPropertiesSystem::AccuracyPress ), "accuracy_press", &mSoldierModel )
    , mSpeedPressModel( StringFunc ( this, &SoldierPropertiesSystem::SpeedPress ), "speed_press", &mSoldierModel )
{
    mOnSoldierCreatedEvent = EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &SoldierPropertiesSystem::OnSoldierCreatedEvent, this, _1 ) );
}


void SoldierPropertiesSystem::Init()
{
    mPropertyModels.push_back( new ModelValue( RefTo( mProgramState.mSoldierProperties.mHealth ), "health", &mSoldierModel ) );
    mPropertyModels.push_back( new ModelValue( RefTo( mProgramState.mSoldierProperties.mAccuracy ), "accuracy", &mSoldierModel ) );
    mPropertyModels.push_back( new ModelValue( RefTo( mProgramState.mSoldierProperties.mMoveSpeed ), "speed", &mSoldierModel ) );
    mPropertyModels.push_back( new ModelValue( RefTo( mProgramState.mSoldierProperties.mPoints ), "points", &mSoldierModel ) );
    mPropertyModels.push_back( new ModelValue( VoidFunc( this, &SoldierPropertiesSystem::OnSoldierPropertiesReady ), "soldier_properties.ready", &RootModel::Get() ) );
}


void SoldierPropertiesSystem::Update( double DeltaTime )
{

}

void SoldierPropertiesSystem::HealthPress( std::string modifier )
{
    ModifyPoints( mProgramState.mSoldierProperties.mHealth, modifier );
}

void SoldierPropertiesSystem::AccuracyPress( std::string modifier )
{
    ModifyPoints( mProgramState.mSoldierProperties.mAccuracy, modifier );
}

void SoldierPropertiesSystem::SpeedPress( std::string modifier )
{
    ModifyPoints( mProgramState.mSoldierProperties.mMoveSpeed, modifier );
}

bool SoldierPropertiesSystem::ModifyPoints( int32_t& currentProperty, std::string modifier )
{
    bool addition = modifier == "+";

    if( addition && currentProperty < mProgramState.mSoldierProperties.mPoints )
    {
        ++currentProperty;
        mProgramState.mSoldierProperties.mPoints -= currentProperty;
        return true;
    }
    if ( !addition && currentProperty > 0 )
    {
        mProgramState.mSoldierProperties.mPoints += currentProperty;
        --currentProperty;
        return true;
    }
    return false;
}

SoldierPropertiesSystem::~SoldierPropertiesSystem()
{
    mPropertyModels.clear();
}

void SoldierPropertiesSystem::OnSoldierPropertiesReady()
{
    if ( mProgramState.mMode != core::ProgramState::Local )
    {
        //TODO client_workflow_system to handle this.
        Ui::Get().Load( "waiting_start" );
    }
    EventServer<SoldierPropertiesReadyEvent>::Get().SendEvent( SoldierPropertiesReadyEvent() );
}

void SoldierPropertiesSystem::OnSoldierCreatedEvent( engine::SoldierCreatedEvent const& Evt )
{
    if (Evt.mState != engine::SoldierCreatedEvent::Raw)
    {
        return;
    }
    if( mProgramState.mMode != core::ProgramState::Client )
    {
        Opt<IBuffHolderComponent> buffHolderC = Evt.mActor->Get<IBuffHolderComponent>();
        if( buffHolderC.IsValid() )
        {
            std::auto_ptr<Buff> buff( core::BuffFactory::Get()( MoveSpeedBuff::GetType_static() ) );
            MoveSpeedBuff* moveSpeedBuff = ( MoveSpeedBuff* )buff.get();
            moveSpeedBuff->SetAutoRemove( false );
            moveSpeedBuff->SetPercentBonus( Evt.mClientData.mSoldierProperties.mMoveSpeed * 3 );
            buffHolderC->AddBuff( buff );

            buff = core::BuffFactory::Get()( MaxHealthBuff::GetType_static() );
            MaxHealthBuff* maxHealthBuff = ( MaxHealthBuff* )buff.get();
            maxHealthBuff->SetFlatBonus( Evt.mClientData.mSoldierProperties.mHealth * 15 );
            maxHealthBuff->SetAutoRemove( false );
            buffHolderC->AddBuff( buff );
            engine::MaxHealthBuffSubSystem::RecalculateBuffs( *Evt.mActor );
            Opt<IHealthComponent> healthC = Evt.mActor->Get<IHealthComponent>();
            healthC->SetHP( healthC->GetMaxHP().Get() );
            L2( "setting health to hp:%d, maxHP calculated:%d guid: %d\n", healthC->GetHP(), healthC->GetMaxHP().Get(), Evt.mActor->GetGUID() );

            buff = core::BuffFactory::Get()( AccuracyBuff::GetType_static() );
            AccuracyBuff* accuracyBuff = ( AccuracyBuff* )buff.get();
            accuracyBuff->SetFlatBonus( Evt.mClientData.mSoldierProperties.mAccuracy * 50 );
            accuracyBuff->SetAutoRemove( false );
            buffHolderC->AddBuff( buff );
            Opt<IAccuracyComponent> accuracyC = Evt.mActor->Get<IAccuracyComponent>();
            engine::AccuracyBuffSubSystem::RecalculateBuffs( *Evt.mActor );
            L2( "setting accuracy calculated:%d guid: %d\n", accuracyC->GetAccuracy().Get(), Evt.mActor->GetGUID() );

        }

    }
}


} // namespace engine

