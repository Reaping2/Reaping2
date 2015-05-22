#include "engine/buffs_engine/max_health_buff_sub_system.h"
#include "core/buffs/max_health_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_health_component.h"

namespace engine {

MaxHealthBuffSubSystem::MaxHealthBuffSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void MaxHealthBuffSubSystem::Init()
{
}


void MaxHealthBuffSubSystem::Update(Actor& actor, double DeltaTime)
{
    RecalculateBuffs(actor);
}

void MaxHealthBuffSubSystem::RecalculateBuffs(Actor &actor)
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if (!buffHolderC.IsValid())
    {
        return;
    }
    Opt<IHealthComponent> healthC=actor.Get<IHealthComponent>();
    if(!healthC.IsValid())
    {
        return;
    }
    healthC->GetMaxHP().mFlat.Set(0.0);
    healthC->GetMaxHP().mPercent.Set(0.0);
    BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffHolderC->GetBuffList(),MaxHealthBuff::GetType_static());
    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator maxHealthBuffIt = buffListFilter.begin(), maxHealthBuffE = buffListFilter.end(); maxHealthBuffIt != maxHealthBuffE; ++maxHealthBuffIt )
    {
        Opt<MaxHealthBuff> maxHealthBuff(*maxHealthBuffIt);
        healthC->GetMaxHP().mFlat.Set(healthC->GetMaxHP().mFlat.Get()+maxHealthBuff->GetFlatBonus());
        healthC->GetMaxHP().mPercent.Set(healthC->GetMaxHP().mPercent.Get()+maxHealthBuff->GetPercentBonus());
    }
}

} // namespace engine

