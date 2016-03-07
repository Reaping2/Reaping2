#include "engine/buffs_engine/accuracy_buff_sub_system.h"
#include "core/buffs/accuracy_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_accuracy_component.h"

namespace engine {

AccuracyBuffSubSystem::AccuracyBuffSubSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}


void AccuracyBuffSubSystem::Init()
{
}


void AccuracyBuffSubSystem::Update( Actor& actor, double DeltaTime )
{
    RecalculateBuffs( actor );
}

void AccuracyBuffSubSystem::RecalculateBuffs( Actor& actor )
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if ( !buffHolderC.IsValid() )
    {
        return;
    }
    Opt<IAccuracyComponent> accuracyC = actor.Get<IAccuracyComponent>();
    if( !accuracyC.IsValid() )
    {
        return;
    }
    accuracyC->GetAccuracy().mFlat.Set( 0.0 );
    accuracyC->GetAccuracy().mPercent.Set( 0.0 );
    BuffListFilter<IBuffHolderComponent::All> buffListFilter( buffHolderC->GetBuffList(), AccuracyBuff::GetType_static() );
    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator accuracyBuffIt = buffListFilter.begin(), accuracyBuffE = buffListFilter.end(); accuracyBuffIt != accuracyBuffE; ++accuracyBuffIt )
    {
        Opt<AccuracyBuff> accuracyBuff( *accuracyBuffIt );
        accuracyC->GetAccuracy().mFlat.Set( accuracyC->GetAccuracy().mFlat.Get() + accuracyBuff->GetFlatBonus() );
        accuracyC->GetAccuracy().mPercent.Set( accuracyC->GetAccuracy().mPercent.Get() + accuracyBuff->GetPercentBonus() );
    }
}

} // namespace engine

